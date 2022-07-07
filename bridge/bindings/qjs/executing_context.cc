/*
 * Copyright (C) 2021-present The Kraken authors. All rights reserved.
 */

#include "executing_context.h"
#include "bindings/qjs/bom/timer.h"
#include "bindings/qjs/bom/window.h"
#include "bindings/qjs/dom/document.h"
#include "bindings/qjs/dom/elements/.gen/canvas_element.h"
#include "bindings/qjs/module_manager.h"
#include "bom/dom_timer_coordinator.h"
#include "foundation/logging.h"
#include "garbage_collected.h"
#include "kraken_bridge.h"
#include "qjs_patch.h"

#include <wchar.h>
#include <codecvt>
#include <iostream>
#include <locale>
#include <string>

namespace kraken::binding::qjs {

static std::atomic<int32_t> context_unique_id{0};

JSClassID ExecutionContext::kHostClassClassId{0};
JSClassID ExecutionContext::kHostObjectClassId{0};
JSClassID ExecutionContext::kHostExoticObjectClassId{0};

std::atomic<int32_t> runningContexts{0};

#define MAX_JS_CONTEXT 1024
bool valid_contexts[MAX_JS_CONTEXT];
std::atomic<uint32_t> running_context_list{0};

std::unique_ptr<ExecutionContext> createJSContext(int32_t contextId, const JSExceptionHandler& handler, void* owner) {
  return std::make_unique<ExecutionContext>(contextId, handler, owner);
}

static JSRuntime* m_runtime{nullptr};

// by bruce
static JSContext* g_context{nullptr};
static ExecutionContext* g_this_ec{nullptr};
static JSValue g_lastobj;
std::unordered_map<int, JSValue> g_jsid2cobj;
static int g_windowid;
static int g_documentid;
static int g_bodyid = 0;

void ExecutionContextGCTracker::trace(JSRuntime* rt, JSValue val, JS_MarkFunc* mark_func) const {
  auto* context = static_cast<ExecutionContext*>(JS_GetContextOpaque(m_ctx));
  context->trace(rt, context->global(), mark_func);
}
void ExecutionContextGCTracker::dispose() const {}

JSClassID ExecutionContextGCTracker::contextGcTrackerClassId{0};

int add_native(wasm_exec_env_t exec_env, int a, int b) {
  return a + b;
}

void abort(wasm_exec_env_t exec_env, int a, int b, int c, int d) {
  return;
}

void trackWindow(wasm_exec_env_t exec_env, int windowid) {
  KRAKEN_LOG(DEBUG) << "  in trackWindow windowid: " << windowid << std::endl;
  g_windowid = windowid;
  return;
}

void getDocument(wasm_exec_env_t exec_env, int windowid, int documentid) {
  KRAKEN_LOG(DEBUG) << "  in getDocument windowid: " << windowid << "  documentid:" << documentid << std::endl;
  g_documentid = documentid;
  g_jsid2cobj[documentid] = g_this_ec->getGlobalProperty("document");
  return;
}

void getCustomElements(wasm_exec_env_t exec_env, int a, int b) {
  KRAKEN_LOG(DEBUG) << "  in getCustomElements a: " << a << "  b:" << b << std::endl;
  return;
}

void getHistory(wasm_exec_env_t exec_env, int a, int b) {
  KRAKEN_LOG(DEBUG) << "  in getHistory a: " << a << "  b:" << b << std::endl;
  return;
}

void trackNextRef(wasm_exec_env_t exec_env, int id) {
  KRAKEN_LOG(DEBUG) << "  in trackNextRef id: " << id << std::endl;
  if (g_bodyid == -1) {
    g_bodyid = id;
    JSValue body = Document::getBody(g_context, g_this_ec->getGlobalProperty("document"), 0, nullptr);
    g_jsid2cobj[id] = body;
    KRAKEN_LOG(DEBUG) << "  in documentbody tag: " << g_jsid2cobj[id].tag << "  value:" << g_jsid2cobj[id].u.ptr << std::endl;
    return;
  }
  g_jsid2cobj[id] = g_lastobj;
  return;
}

int createTextNode(wasm_exec_env_t exec_env, int id, char16_t* data) {
  // std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> converter;

  std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
  int* datalen = (int*)data - 1;
  std::string datastr = converter.to_bytes(data, data + (*datalen) / 2);

  JSValue str = JS_NewString(g_context, datastr.c_str());

  KRAKEN_LOG(DEBUG) << "  in createTextNode id: " << id << "  data:" << datastr << "  datalen:" << *datalen << std::endl;
  /*
  unsigned char* data1 = (unsigned char*)data;
  KRAKEN_LOG(DEBUG) << "  in createTextNode id: "  << id  <<   "  data:"   <<  std::hex <<  data1[0] << " " << data1[1]  << " "<<  data1[2] << " " << data1[3]  << " "
         <<  data1[4] << " " << data1[5]  << " " <<  data1[6] << " " << data1[7]   << " " <<  data1[8] << " " << data1[9]  << " "<<  data1[10] << " " << data1[11]   << " "
         <<  data1[12] << " " << data1[13]  << " " <<  data1[14] << " " << data1[15]   << " "  <<  data1[16] << " " << data1[17]  << " " <<  data1[18] << " " << data1[19]  << " "
         <<  data1[20] << " " << data1[21]  << " " <<  data1[22] << " " << data1[23]   << " " <<  data1[24] << " " << data1[25]  << " "<<  data1[26] << " " << data1[27] << " "
          <<  data1[28] << " " << data1[29]  << " "<<  data1[30] << " " << data1[31]   << " " <<  data1[32] << " " << data1[33]  << " "<<  data1[34] << " " << data1[35]<< std::endl;

      KRAKEN_LOG(DEBUG) << "  in createTextNode id: "  << id  <<   "  data:"   <<  std::hex <<  (unsigned int)data1[0] << " " <<  (unsigned int)data1[1]  << " "<<   (unsigned int)data1[2] << " " <<
  (unsigned int)data1[3]  << " "
         <<   (unsigned int)data1[4] << " " <<  (unsigned int)data1[5]  << " "<<   (unsigned int)data1[6] << " " <<  (unsigned int)data1[7]    << " "<<   (unsigned int)data1[8] << " " <<  (unsigned
  int)data1[9]  << " " <<   (unsigned int)data1[10] << " " << (unsigned int) data1[11]   << " "
         <<   (unsigned int)data1[12] << " " <<  (unsigned int)data1[13] << " " <<  (unsigned int) data1[14] << " " <<  (unsigned int)data1[15]  << " "  << (unsigned int)  data1[16] << " " <<
  (unsigned int)data1[17]  << " " <<   (unsigned int)data1[18] << " " << (unsigned int) data1[19]  << " "
         <<   (unsigned int)data1[20] << " " <<  (unsigned int)data1[21] << " " <<  (unsigned int) data1[22] << " " <<  (unsigned int)data1[23]   << " " <<   (unsigned int)data1[24] << " " <<
  (unsigned int)data1[25]  << " " <<   (unsigned int)data1[26] << " " << (unsigned int) data1[27] << " "
          <<   (unsigned int)data1[28] << " " << (unsigned int) data1[29]  << " "<<  (unsigned int) data1[30] << " " <<  (unsigned int)data1[31]    << " " <<   (unsigned int)data1[32] << " " <<
  (unsigned int)data1[33] << " " <<   (unsigned int)data1[34] << " " <<  (unsigned int)data1[35]<< std::endl;
  */
  g_lastobj = Document::createTextNode(g_context, g_jsid2cobj[id], 1, &str);

  JS_FreeValue(g_context, str);

  return -100;
}

int createElement(wasm_exec_env_t exec_env, int id, char16_t* data) {
  std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> converter;

  int* datalen = (int*)data - 1;
  std::string datastr = converter.to_bytes(data, data + (*datalen) / 2);

  JSValue str = JS_NewString(g_context, datastr.c_str());

  KRAKEN_LOG(DEBUG) << "  in createElement id: " << id << "  data:" << datastr << std::endl;

  g_lastobj = Document::createElement(g_context, g_jsid2cobj[id], 1, &str);

  if (g_lastobj.u.ptr == nullptr) {
    KRAKEN_LOG(DEBUG) << "  in createElement create element faild: "
                      << "  data:" << datastr << std::endl;
    return 0;
  }

  JS_FreeValue(g_context, str);

  if (datastr == "p" || datastr == "br") {
    return -5;
  }
  return 0;
}

int getElementById(wasm_exec_env_t exec_env, int id, char16_t* data) {
  std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> converter;
  int* datalen = (int*)data - 1;
  std::string datastr = converter.to_bytes(data, data + (*datalen) / 2);
  KRAKEN_LOG(DEBUG) << "  in getElementById id: " << datastr << std::endl;
  JSValue str = JS_NewString(g_context, datastr.c_str());
  g_lastobj = Document::getElementById(g_context, g_jsid2cobj[id], 1, &str);
  JS_FreeValue(g_context, str);

  if (g_lastobj.tag == JS_TAG_OBJECT && g_lastobj.u.ptr != nullptr) {
    auto* instance = static_cast<ElementInstance*>(JS_GetOpaque(g_lastobj, Element::kElementClassId));
    KRAKEN_LOG(DEBUG) << "  in getElementById ElementInstance tagname: " << instance->tagName() << std::endl;
    if (instance->tagName() == "CANVAS") {
      return -17;
    }
  }

  return 0;
}

void getContext(wasm_exec_env_t exec_env, int canvasid, int ctxId, int typeNum) {
  KRAKEN_LOG(DEBUG) << "  in CanvasGetContext canvasid: " << canvasid << "  canvas prt:" << g_jsid2cobj[canvasid].u.ptr << "   ctxId:" << ctxId << "  typeNum:" << typeNum << std::endl;
  if (typeNum != 0)
    return;
  JSValue str = JS_NewString(g_context, "2d");
  g_jsid2cobj[ctxId] = CanvasElement::getContext(g_context, g_jsid2cobj[canvasid], 1, &str);
  JS_FreeValue(g_context, str);
  return;
}

void setfillStyle(wasm_exec_env_t exec_env, int ctxId, char16_t* style) {
  std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> converter;
  int* datalen = (int*)style - 1;
  std::string datastr = converter.to_bytes(style, style + (*datalen) / 2);
  KRAKEN_LOG(DEBUG) << "  in setfillStyle id: " << style << std::endl;
  // JSValue  attrV = JS_NewString(g_context,datastr.c_str());
  // CanvasRenderingContext2D::fillStylePropertyDescriptor::setter(g_context,g_jsid2cobj[ctxId],1,&attrV);

  auto* object = static_cast<CanvasRenderingContext2D*>(JS_GetOpaque(g_jsid2cobj[ctxId], ExecutionContext::kHostObjectClassId));
  getDartMethod()->flushUICommand();
  // JSValue value = argv[0];

  // const char* stringValue = JS_ToCString(g_context, attrV);
  object->setBindingProperty("fillStyle", Native_NewCString(datastr.c_str()));
  // JS_FreeCString(g_context, stringValue);

  // JS_FreeValue(g_context,attrV);
}

void fillRect(wasm_exec_env_t exec_env, int ctxid, int x, int y, int width, int height) {
  KRAKEN_LOG(DEBUG) << "  in Canvas2DContextfillRect ctxid: " << ctxid << "  x:" << x << "   y:" << y << "  width:" << width << "  height:" << height << std::endl;
  JSValue argc[4];
  argc[0].tag = JS_TAG_INT;
  argc[0].u.int32 = x;
  argc[1].tag = JS_TAG_INT;
  argc[1].u.int32 = y;
  argc[2].tag = JS_TAG_INT;
  argc[2].u.int32 = width;
  argc[3].tag = JS_TAG_INT;
  argc[3].u.int32 = height;
  CanvasRenderingContext2D::fillRect(g_context, g_jsid2cobj[ctxid], 4, argc);
  return;
}

void log(wasm_exec_env_t exec_env, char16_t* data) {
  std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> converter;
  int* datalen = (int*)data - 1;
  std::string datastr = converter.to_bytes(data, data + (*datalen) / 2);

  KRAKEN_LOG(DEBUG) << "  in mylog: " << datastr << std::endl;
  return;
}

void nodeAppendChild(wasm_exec_env_t exec_env, int parentId, int childId) {
  KRAKEN_LOG(DEBUG) << "  in nodeAppendChild pid: " << parentId << "  cid:" << childId << std::endl;
  JSValue ret = Node::appendChild(g_context, g_jsid2cobj[parentId], 1, &g_jsid2cobj[childId]);
  KRAKEN_LOG(DEBUG) << "  in nodeAppendChild ret: " << ret.tag << "  ret val:" << ret.u.ptr << std::endl;
}

inline std::string trim(std::string& str) {
  str.erase(0, str.find_first_not_of(' '));  // prefixing spaces
  str.erase(str.find_last_not_of(' ') + 1);  // surfixing spaces
  return str;
}

void elSetAttribute(wasm_exec_env_t exec_env, int id, char16_t* attr, char16_t* value) {
  std::wstring_convert<std::codecvt_utf8<char16_t>, char16_t> converter;
  int* datalen = (int*)attr - 1;
  std::string attrstr = converter.to_bytes(attr, attr + (*datalen) / 2);
  int* datalen1 = (int*)value - 1;
  std::string valuestr = converter.to_bytes(value, value + (*datalen1) / 2);

  KRAKEN_LOG(DEBUG) << "  in elSetAttribute id: " << id << "  attr:" << attrstr << "   value:" << valuestr << std::endl;

  JSValue attrV[2];

  JSValue ret;
  if (attrstr == "style") {
    std::vector<std::string> arrStyles;
    std::string::size_type prev_pos = 0, pos = 0;
    std::string strStyles = valuestr;

    while ((pos = strStyles.find(';', pos)) != std::string::npos) {
      arrStyles.push_back(strStyles.substr(prev_pos, pos - prev_pos));
      prev_pos = ++pos;
    }
    arrStyles.push_back(strStyles.substr(prev_pos, pos - prev_pos));

    KRAKEN_LOG(DEBUG) << "  arrStyles count: " << arrStyles.size() << std::endl;

    for (auto& s : arrStyles) {
      std::string::size_type position = s.find(':');
      if (position != std::basic_string<char>::npos) {
        std::string styleKey = s.substr(0, position);
        trim(styleKey);
        std::string styleValue = s.substr(position + 1, s.length());
        trim(styleValue);

        attrV[0] = JS_NewString(g_context, styleKey.c_str());
        attrV[1] = JS_NewString(g_context, styleValue.c_str());

        // style->internalSetProperty(styleKey, newStyleValue);
        auto* instance = static_cast<ElementInstance*>(JS_GetOpaque(g_jsid2cobj[id], Element::kElementClassId));
        // KRAKEN_LOG(DEBUG) << "  style key: "  <<  styleKey  <<   "  style value:" << styleValue  <<   "   instance:" <<   instance  << std::endl;

        ret = CSSStyleDeclaration::setProperty(g_context, instance->style()->jsObject, 2, attrV);
        JS_FreeValue(g_context, attrV[0]);
        JS_FreeValue(g_context, attrV[1]);
      }
    }

  } else {
    attrV[0] = JS_NewString(g_context, attrstr.c_str());
    attrV[1] = JS_NewString(g_context, valuestr.c_str());
    ret = Element::setAttribute(g_context, g_jsid2cobj[id], 2, attrV);
    JS_FreeValue(g_context, attrV[0]);
    JS_FreeValue(g_context, attrV[1]);
  }

  KRAKEN_LOG(DEBUG) << "  in elSetAttribute ret tag: " << ret.tag << "  value:" << ret.u.ptr << std::endl;

  return;
}

int getBody(wasm_exec_env_t exec_env, int id) {
  KRAKEN_LOG(DEBUG) << "  in getBody documentid: " << id << std::endl;
  if (g_bodyid == 0) {
    g_bodyid = -1;
    return -2;
  }
  return g_bodyid;
}

static JSValue js_call_warm_func(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv, int magic, JSValue* func_data) {
  KRAKEN_LOG(DEBUG) << "  in js_call_warm_func this_val tag: " << this_val.tag << "  data:" << this_val.u.ptr << std::endl;
  const char* funcname = JS_ToCString(ctx, func_data[1]);
  ExecutionContext* ptr = (ExecutionContext*)(func_data[0].u.ptr);

  wasm_function_inst_t func = wasm_runtime_lookup_function(ptr->module_inst, funcname, NULL);

  if (func == nullptr) {
    std::string errormsg = funcname;
    errormsg = errormsg + " no found";
    KRAKEN_LOG(DEBUG) << "  could not found func: " << funcname << std::endl;
    return JS_NewString(ctx, errormsg.c_str());
  }

  wasm_val_t results[1];
  wasm_val_t* wasm_argv = new wasm_val_t[argc];
  for (int i = 0; i < argc; ++i) {
    // KRAKEN_LOG(DEBUG) << "  arg type " << argv[i].tag   << std::endl;
    if (argv[i].tag == JS_TAG_FLOAT64) {
      wasm_argv[i].kind = WASM_F64;
      JS_ToFloat64(ctx, &(wasm_argv[i].of.f64), argv[i]);
    } else if (argv[i].tag == JS_TAG_INT) {
      wasm_argv[i].kind = WASM_I32;
      JS_ToInt32(ctx, &(wasm_argv[i].of.i32), argv[i]);
    }
  }

  if (wasm_runtime_call_wasm_a(ptr->exec_env, func, 1, results, argc, wasm_argv)) {
    KRAKEN_LOG(DEBUG) << "  result type: " << (int)results[0].kind << std::endl;
    if (results[0].kind == WASM_I32)
      return JS_NewInt32(ctx, results[0].of.i32);
    else if (results[0].kind == WASM_F64)
      return JS_NewFloat64(ctx, results[0].of.f64);
    else if (results[0].kind == WASM_F32)
      return JS_NewFloat64(ctx, results[0].of.f32);
  } else {
    KRAKEN_LOG(DEBUG) << wasm_runtime_get_exception(ptr->module_inst) << std::endl;
  }
  return JS_NewError(ctx);
  /*for(int i = 0; i < argc; i++)
  {
    KRAKEN_LOG(DEBUG) << "  js myadd argv: " << argv[i].tag    << std::endl;
  }
  if(strcmp("myadd",funcname)==0)
  {
    int a = 0;
    int b = 0;
    JS_ToInt32(ctx, &a, argv[0]);
    JS_ToInt32(ctx, &b, argv[1]);
    return JS_NewInt32(ctx, a + b);
  }
  else     if(strcmp("myadd2",funcname)==0)
  {
    int a = 0;
    int b = 0;
    int c = 0;
    JS_ToInt32(ctx, &a, argv[0]);
    JS_ToInt32(ctx, &b, argv[1]);
    JS_ToInt32(ctx, &c, argv[2]);
    return JS_NewInt32(ctx, a + b+c);
  }*/
  // return a+1
}

ExecutionContext::ExecutionContext(int32_t contextId, const JSExceptionHandler& handler, void* owner)
    : contextId(contextId), _handler(handler), owner(owner), ctxInvalid_(false), uniqueId(context_unique_id++) {
  // @FIXME: maybe contextId will larger than MAX_JS_CONTEXT
  KRAKEN_LOG(DEBUG) << "  ExecutionContext start!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << std::endl;

  valid_contexts[contextId] = true;
  if (contextId > running_context_list)
    running_context_list = contextId;

  std::call_once(kinitJSClassIDFlag, []() {
    JS_NewClassID(&kHostClassClassId);
    JS_NewClassID(&kHostObjectClassId);
    JS_NewClassID(&kHostExoticObjectClassId);
  });

  init_list_head(&node_job_list);
  init_list_head(&module_job_list);
  init_list_head(&module_callback_job_list);
  init_list_head(&promise_job_list);
  init_list_head(&native_function_job_list);

  if (m_runtime == nullptr) {
    m_runtime = JS_NewRuntime();
  }
  // Avoid stack overflow when running in multiple threads.
  JS_UpdateStackTop(m_runtime);
  m_ctx = JS_NewContext(m_runtime);

  timeOrigin = std::chrono::system_clock::now();
  globalObject = JS_GetGlobalObject(m_ctx);
  JSValue windowGetter = JS_NewCFunction(
      m_ctx, [](JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) -> JSValue { return JS_GetGlobalObject(ctx); }, "get", 0);
  JSAtom windowKey = JS_NewAtom(m_ctx, "window");
  JS_DefinePropertyGetSet(m_ctx, globalObject, windowKey, windowGetter, JS_UNDEFINED, JS_PROP_HAS_GET | JS_PROP_ENUMERABLE);
  JS_FreeAtom(m_ctx, windowKey);
  JS_SetContextOpaque(m_ctx, this);
  JS_SetHostPromiseRejectionTracker(m_runtime, promiseRejectTracker, nullptr);

  m_gcTracker = makeGarbageCollected<ExecutionContextGCTracker>()->initialize(m_ctx, &ExecutionContextGCTracker::contextGcTrackerClassId);
  JS_DefinePropertyValueStr(m_ctx, globalObject, "_gc_tracker_", m_gcTracker->toQuickJS(), JS_PROP_NORMAL);

  // by bruce
  g_context = m_ctx;
  g_this_ec = this;
  wasm_runtime_init();

  static NativeSymbol native_symbols_env[] = {EXPORT_WASM_API_WITH_SIG(abort, "(iiii)")};
  int n_native_symbols_env = sizeof(native_symbols_env) / sizeof(NativeSymbol);
  if (!wasm_runtime_register_natives("env", native_symbols_env, n_native_symbols_env)) {
    KRAKEN_LOG(DEBUG) << "   wasm  register  native method fail" << std::endl;
  }

  static NativeSymbol native_symbols_window[] = {EXPORT_WASM_API_WITH_SIG(trackWindow, "(i)"), EXPORT_WASM_API_WITH_SIG(getDocument, "(ii)"), EXPORT_WASM_API_WITH_SIG(getCustomElements, "(ii)"),
                                                 EXPORT_WASM_API_WITH_SIG(getHistory, "(ii)")};
  int n_native_symbols_window = sizeof(native_symbols_window) / sizeof(NativeSymbol);
  if (!wasm_runtime_register_natives("asDOM_Window", native_symbols_window, n_native_symbols_window)) {
    KRAKEN_LOG(DEBUG) << "   wasm  register  native method fail" << std::endl;
  }

  static NativeSymbol native_symbols[] = {EXPORT_WASM_API_WITH_SIG(createTextNode, "(i$)i"), EXPORT_WASM_API_WITH_SIG(createElement, "(i$)i"), EXPORT_WASM_API_WITH_SIG(getElementById, "(i$)i"),
                                          EXPORT_WASM_API_WITH_SIG(getBody, "(i)i")};
  int n_native_symbols = sizeof(native_symbols) / sizeof(NativeSymbol);
  if (!wasm_runtime_register_natives("asDOM_Document", native_symbols, n_native_symbols)) {
    KRAKEN_LOG(DEBUG) << "   wasm  register  native method fail" << std::endl;
  }

  static NativeSymbol native_symbols_node[] = {EXPORT_WASM_API_WITH_SIG(nodeAppendChild, "(ii)")};
  int n_native_symbols_node = sizeof(native_symbols_node) / sizeof(NativeSymbol);
  if (!wasm_runtime_register_natives("asDOM_Node", native_symbols_node, n_native_symbols_node)) {
    KRAKEN_LOG(DEBUG) << "   wasm  register  native method fail" << std::endl;
  }

  static NativeSymbol native_symbols_element[] = {EXPORT_WASM_API_WITH_SIG(elSetAttribute, "(i$$)")};
  int n_native_symbols_element = sizeof(native_symbols_element) / sizeof(NativeSymbol);
  if (!wasm_runtime_register_natives("asDOM_Element", native_symbols_element, n_native_symbols_element)) {
    KRAKEN_LOG(DEBUG) << "   wasm  register  native method fail" << std::endl;
  }

  static NativeSymbol native_symbols_asdom[] = {EXPORT_WASM_API_WITH_SIG(trackNextRef, "(i)"), EXPORT_WASM_API_WITH_SIG(log, "($)")};
  int n_native_symbols_asdom = sizeof(native_symbols_asdom) / sizeof(NativeSymbol);
  if (!wasm_runtime_register_natives("asDOM", native_symbols_asdom, n_native_symbols_asdom)) {
    KRAKEN_LOG(DEBUG) << "   wasm  register  native method fail" << std::endl;
  }

  static NativeSymbol native_symbols_asdom_htmlcanvas[] = {EXPORT_WASM_API_WITH_SIG(getContext, "(iii)")};
  int n_native_symbols_asdom_htmlcanvas = sizeof(native_symbols_asdom_htmlcanvas) / sizeof(NativeSymbol);
  if (!wasm_runtime_register_natives("asDOM_HTMLCanvasElement", native_symbols_asdom_htmlcanvas, n_native_symbols_asdom_htmlcanvas)) {
    KRAKEN_LOG(DEBUG) << "   wasm  register  native method fail" << std::endl;
  }

  static NativeSymbol native_symbols_asdom_2dcanvascontext[] = {EXPORT_WASM_API_WITH_SIG(setfillStyle, "(i$)"), EXPORT_WASM_API_WITH_SIG(fillRect, "(iiiii)")};
  int n_native_symbols_asdom_2dcanvascontext = sizeof(native_symbols_asdom_2dcanvascontext) / sizeof(NativeSymbol);
  if (!wasm_runtime_register_natives("asDOM_Canvas2DRenderingContext", native_symbols_asdom_2dcanvascontext, n_native_symbols_asdom_2dcanvascontext)) {
    KRAKEN_LOG(DEBUG) << "   wasm  register  native method fail" << std::endl;
  }
  runningContexts++;
}

ExecutionContext::~ExecutionContext() {
  valid_contexts[contextId] = false;
  ctxInvalid_ = true;

  KRAKEN_LOG(DEBUG) << "  ExecutionContext finish!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! " << std::endl;
  // Manual free nodes bound by each other.
  {
    struct list_head *el, *el1;
    list_for_each_safe(el, el1, &node_job_list) {
      auto* node = list_entry(el, NodeJob, link);
      JS_FreeValue(m_ctx, node->nodeInstance->jsObject);
    }
  }

  // Manual free moduleListener
  {
    struct list_head *el, *el1;
    list_for_each_safe(el, el1, &module_job_list) {
      auto* module = list_entry(el, ModuleContext, link);
      JS_FreeValue(m_ctx, module->callback);
      delete module;
    }
  }

  {
    struct list_head *el, *el1;
    list_for_each_safe(el, el1, &module_callback_job_list) {
      auto* module = list_entry(el, ModuleContext, link);
      JS_FreeValue(m_ctx, module->callback);
      delete module;
    }
  }

  // Free unresolved promise.
  {
    struct list_head *el, *el1;
    list_for_each_safe(el, el1, &promise_job_list) {
      auto* promiseContext = list_entry(el, PromiseContext, link);
      JS_FreeValue(m_ctx, promiseContext->resolveFunc);
      JS_FreeValue(m_ctx, promiseContext->rejectFunc);
      delete promiseContext;
    }
  }

  // Free unreleased native_functions.
  {
    struct list_head *el, *el1;
    list_for_each_safe(el, el1, &native_function_job_list) {
      auto* job = list_entry(el, NativeFunctionContext, link);
      delete job;
    }
  }

  // Check if current context have unhandled exceptions.
  JSValue exception = JS_GetException(m_ctx);
  if (JS_IsObject(exception) || JS_IsException(exception)) {
    // There must be bugs in native functions from call stack frame. Someone needs to fix it if throws.
    reportError(exception);
    assert_m(false, "Unhandled exception found when dispose JSContext.");
  }

  JS_FreeValue(m_ctx, globalObject);
  JS_FreeContext(m_ctx);

  // Run GC to clean up remaining objects about m_ctx;
  JS_RunGC(m_runtime);

  // by bruce
  if (m_loadwasm) {
    wasm_runtime_destroy_exec_env(exec_env);
    wasm_runtime_deinstantiate(module_inst);
    wasm_runtime_unload(module);
    delete[] m_wasmbuf;
    m_wasmbuf = nullptr;
  }
  wasm_runtime_destroy();

#if DUMP_LEAKS
  if (--runningContexts == 0) {
    JS_FreeRuntime(m_runtime);
    m_runtime = nullptr;
  }
#endif
  m_ctx = nullptr;
}

bool ExecutionContext::evaluateJavaScript(const uint16_t* code, size_t codeLength, const char* sourceURL, int startLine) {
  std::string utf8Code = toUTF8(std::u16string(reinterpret_cast<const char16_t*>(code), codeLength));
  // bruce
  // KRAKEN_LOG(DEBUG) <<  "this1: "   <<  this << "  js utf8Code code: " << utf8Code  <<  "  codeLength:"  << codeLength   << std::endl;
  JSValue result = JS_Eval(m_ctx, utf8Code.c_str(), utf8Code.size(), sourceURL, JS_EVAL_TYPE_GLOBAL);
  drainPendingPromiseJobs();
  bool success = handleException(&result);
  JS_FreeValue(m_ctx, result);
  return success;
}

bool ExecutionContext::evaluateJavaScript(const char16_t* code, size_t length, const char* sourceURL, int startLine) {
  std::string utf8Code = toUTF8(std::u16string(reinterpret_cast<const char16_t*>(code), length));
  // bruce
  // KRAKEN_LOG(DEBUG) <<  "this2: "   <<  this << "  js utf8Code code: " << utf8Code  <<  "  codeLength:"  << length   << std::endl;
  JSValue result = JS_Eval(m_ctx, utf8Code.c_str(), utf8Code.size(), sourceURL, JS_EVAL_TYPE_GLOBAL);
  drainPendingPromiseJobs();
  bool success = handleException(&result);
  JS_FreeValue(m_ctx, result);
  return success;
}

bool ExecutionContext::evaluateJavaScript(const char* code, size_t codeLength, const char* sourceURL, int startLine) {
  // bruce
  // KRAKEN_LOG(DEBUG) <<  "this: "   <<  this << "  js code: " << code  <<  "  codeLength:"  << codeLength   << std::endl;
  JSValue result = JS_Eval(m_ctx, code, codeLength, sourceURL, JS_EVAL_TYPE_GLOBAL);
  drainPendingPromiseJobs();
  bool success = handleException(&result);
  JS_FreeValue(m_ctx, result);
  return success;
}

bool ExecutionContext::evaluateByteCode(uint8_t* bytes, size_t byteLength) {
  JSValue obj, val;
  obj = JS_ReadObject(m_ctx, bytes, byteLength, JS_READ_OBJ_BYTECODE);
  if (!handleException(&obj))
    return false;
  val = JS_EvalFunction(m_ctx, obj);
  if (!handleException(&val))
    return false;
  JS_FreeValue(m_ctx, val);
  return true;
}

// bruce
bool ExecutionContext::evaluateWasmByteCode(uint8_t* bytes, size_t byteLength) {
  KRAKEN_LOG(DEBUG) << "this: " << this << "  wasm code: " << bytes << "  byteLength:" << byteLength << std::endl;
  m_wasmbuf = new unsigned char[byteLength];
  memcpy(m_wasmbuf, bytes, byteLength);
  char error_buf[128] = {"no error!"};
  /*wasm_module_t module;
  wasm_module_inst_t module_inst;
  wasm_function_inst_t func;
  wasm_exec_env_t exec_env;*/
  unsigned int size, stack_size = 8092, heap_size = 8092;

  KRAKEN_LOG(DEBUG) << "before  wasm_runtime_load " << std::endl;
  module = wasm_runtime_load(m_wasmbuf, byteLength, error_buf, sizeof(error_buf));
  KRAKEN_LOG(DEBUG) << "after  wasm_runtime_load  module: " << module << "  error:" << error_buf << std::endl;
  /* create an instance of the WASM module (WASM linear memory is ready) */
  module_inst = wasm_runtime_instantiate(module, stack_size, heap_size, error_buf, sizeof(error_buf));
  KRAKEN_LOG(DEBUG) << "after  wasm_runtime_instantiate  module_inst: " << module_inst << "  error:" << error_buf << std::endl;
  exec_env = wasm_runtime_create_exec_env(module_inst, stack_size);
  KRAKEN_LOG(DEBUG) << "after  wasm_runtime_create_exec_env " << error_buf << std::endl;

  // KRAKEN_LOG(DEBUG) <<  "ExecutionContext:  " <<  this  <<   "  module_inst: "  << module_inst  << std::endl;

  m_loadwasm = true;
  char func_name[2048][512];
  int param_count[2048];
  int func_count = 0;
  // KRAKEN_LOG(DEBUG) <<  "before  wasm_runtime_list_functions " << std::endl;
  func_count = wasm_runtime_list_functions(module_inst, func_name, param_count);
  // KRAKEN_LOG(DEBUG) <<  "after  wasm_runtime_list_functions "   <<  std::endl;

  // KRAKEN_LOG(DEBUG) <<  "wasm_runtime_list_functions ret: "   <<   func_count   << std::endl;

  JSValue global_obj = JS_GetGlobalObject(ctx());
  /*
  JSValue data = (JSValueConst)JS_NewString(ctx(),"add");
  JS_SetPropertyStr(ctx(), global_obj, "add",
                      JS_NewCFunctionData(ctx(), js_call_warm_func, 2,0,1,&data));
  JSValue data2 = (JSValueConst)JS_NewString(ctx(),"add2");
  JS_SetPropertyStr(ctx(), global_obj, "add2",
                      JS_NewCFunctionData(ctx(), js_call_warm_func, 3,0,1,&data2));*/

  JSValueConst data[2];
  data[0].u.ptr = this;

  for (int i = 0; i < func_count; ++i) {
    // KRAKEN_LOG(DEBUG) <<  "func_name: "   <<  func_name[i] << "  param_count: " << param_count[i]    << std::endl;

    data[1] = (JSValueConst)JS_NewString(ctx(), func_name[i]);
    JS_SetPropertyStr(ctx(), global_obj, func_name[i], JS_NewCFunctionData(ctx(), js_call_warm_func, param_count[i], 0, 2, data));
  }

  JS_FreeValue(ctx(), global_obj);

  // wasm_function_inst_t func = wasm_runtime_lookup_function(module_inst, "add", NULL);
  // KRAKEN_LOG(DEBUG) <<  "wasm_runtime_lookup_function: "   <<  func << std::endl;
  /*
    wasm_function_inst_t func = wasm_runtime_lookup_function(module_inst, "add", NULL);
    exec_env = wasm_runtime_create_exec_env(module_inst, stack_size);

    unsigned int argv[2];

    argv[0] = 8;
    argv[1] = -6;
    if(wasm_runtime_call_wasm(exec_env, func, 2, argv))
    {
        KRAKEN_LOG(DEBUG) << "  call wasm add success ret: " << argv[0]    << std::endl;
    }
    else{
        KRAKEN_LOG(DEBUG) << "  call wasm add fail"  << std::endl;
    }*/
  return true;
}

bool ExecutionContext::isValid() const {
  return !ctxInvalid_;
}

int32_t ExecutionContext::getContextId() const {
  assert(!ctxInvalid_ && "context has been released");
  return contextId;
}

void* ExecutionContext::getOwner() {
  assert(!ctxInvalid_ && "context has been released");
  return owner;
}

bool ExecutionContext::handleException(JSValue* exception) {
  if (JS_IsException(*exception)) {
    JSValue error = JS_GetException(m_ctx);
    dispatchGlobalErrorEvent(this, error);
    JS_FreeValue(m_ctx, error);
    return false;
  }

  return true;
}

JSValue ExecutionContext::global() {
  return globalObject;
}

JSContext* ExecutionContext::ctx() {
  assert(!ctxInvalid_ && "context has been released");
  return m_ctx;
}

JSRuntime* ExecutionContext::runtime() {
  return m_runtime;
}

void ExecutionContext::reportError(JSValueConst error) {
  if (!JS_IsError(m_ctx, error))
    return;

  JSValue messageValue = JS_GetPropertyStr(m_ctx, error, "message");
  JSValue errorTypeValue = JS_GetPropertyStr(m_ctx, error, "name");
  const char* title = JS_ToCString(m_ctx, messageValue);
  const char* type = JS_ToCString(m_ctx, errorTypeValue);
  const char* stack = nullptr;
  JSValue stackValue = JS_GetPropertyStr(m_ctx, error, "stack");
  if (!JS_IsUndefined(stackValue)) {
    stack = JS_ToCString(m_ctx, stackValue);
  }

  uint32_t messageLength = strlen(type) + strlen(title);
  if (stack != nullptr) {
    messageLength += 4 + strlen(stack);
    char message[messageLength];
    sprintf(message, "%s: %s\n%s", type, title, stack);
    _handler(contextId, message);
  } else {
    messageLength += 3;
    char message[messageLength];
    sprintf(message, "%s: %s", type, title);
    _handler(contextId, message);
  }

  JS_FreeValue(m_ctx, errorTypeValue);
  JS_FreeValue(m_ctx, messageValue);
  JS_FreeValue(m_ctx, stackValue);
  JS_FreeCString(m_ctx, title);
  JS_FreeCString(m_ctx, stack);
  JS_FreeCString(m_ctx, type);
}

void ExecutionContext::reportErrorEvent(EventInstance* errorEvent) {
  JSValue error = JS_GetPropertyStr(m_ctx, errorEvent->jsObject, "error");
  reportError(error);
  JS_FreeValue(m_ctx, error);
}

void ExecutionContext::dispatchErrorEvent(EventInstance* errorEvent) {
  if (m_inDispatchErrorEvent_) {
    return;
  }

  dispatchErrorEventInternal(errorEvent);
  reportErrorEvent(errorEvent);
}

void ExecutionContext::dispatchErrorEventInternal(EventInstance* errorEvent) {
  if (m_window == nullptr)
    return;

  assert(!m_inDispatchErrorEvent_);
  m_inDispatchErrorEvent_ = true;
  m_window->dispatchEvent(errorEvent);
  m_inDispatchErrorEvent_ = false;
}

void ExecutionContext::drainPendingPromiseJobs() {
  // should executing pending promise jobs.
  JSContext* pctx;
  int finished = JS_ExecutePendingJob(runtime(), &pctx);
  while (finished != 0) {
    finished = JS_ExecutePendingJob(runtime(), &pctx);
    if (finished == -1) {
      break;
    }
  }

  // Throw error when promise are not handled.
  m_rejectedPromise.process(this);
}

void ExecutionContext::defineGlobalProperty(const char* prop, JSValue value) {
  JSAtom atom = JS_NewAtom(m_ctx, prop);
  JS_SetProperty(m_ctx, globalObject, atom, value);
  JS_FreeAtom(m_ctx, atom);
}

JSValue ExecutionContext::getGlobalProperty(const char* prop) {
  return JS_GetPropertyStr(m_ctx, globalObject, prop);
}

uint8_t* ExecutionContext::dumpByteCode(const char* code, uint32_t codeLength, const char* sourceURL, size_t* bytecodeLength) {
  JSValue object = JS_Eval(m_ctx, code, codeLength, sourceURL, JS_EVAL_TYPE_GLOBAL | JS_EVAL_FLAG_COMPILE_ONLY);
  bool success = handleException(&object);
  if (!success)
    return nullptr;
  uint8_t* bytes = JS_WriteObject(m_ctx, bytecodeLength, object, JS_WRITE_OBJ_BYTECODE);
  JS_FreeValue(m_ctx, object);
  return bytes;
}

void ExecutionContext::dispatchGlobalErrorEvent(ExecutionContext* context, JSValueConst error) {
  JSContext* ctx = context->ctx();
  auto* window = static_cast<WindowInstance*>(JS_GetOpaque(context->global(), Window::classId()));

  {
    JSValue errorEventConstructor = JS_GetPropertyStr(ctx, context->global(), "ErrorEvent");
    JSValue errorType = JS_NewString(ctx, "error");
    JSValue errorInit = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, errorInit, "error", JS_DupValue(ctx, error));
    JS_SetPropertyStr(ctx, errorInit, "message", JS_GetPropertyStr(ctx, error, "message"));
    JS_SetPropertyStr(ctx, errorInit, "lineno", JS_GetPropertyStr(ctx, error, "lineNumber"));
    JS_SetPropertyStr(ctx, errorInit, "filename", JS_GetPropertyStr(ctx, error, "fileName"));
    JS_SetPropertyStr(ctx, errorInit, "colno", JS_NewUint32(ctx, 0));
    JSValue arguments[] = {errorType, errorInit};
    JSValue errorEventValue = JS_CallConstructor(context->ctx(), errorEventConstructor, 2, arguments);
    if (JS_IsException(errorEventValue)) {
      context->handleException(&errorEventValue);
      return;
    }

    auto* errorEvent = static_cast<EventInstance*>(JS_GetOpaque(errorEventValue, Event::kEventClassID));
    errorEvent->setTarget(window);
    context->dispatchErrorEvent(errorEvent);

    JS_FreeValue(ctx, errorEventConstructor);
    JS_FreeValue(ctx, errorEventValue);
    JS_FreeValue(ctx, errorType);
    JS_FreeValue(ctx, errorInit);

    context->drainPendingPromiseJobs();
  }
}

static void dispatchPromiseRejectionEvent(const char* eventType, ExecutionContext* context, JSValueConst promise, JSValueConst error) {
  JSContext* ctx = context->ctx();
  auto* window = static_cast<WindowInstance*>(JS_GetOpaque(context->global(), Window::classId()));

  // Trigger PromiseRejectionEvent(unhandledrejection) event.
  {
    JSValue PromiseRejectionEventValue = JS_GetPropertyStr(ctx, context->global(), "PromiseRejectionEvent");
    JSValue errorType = JS_NewString(ctx, eventType);
    JSValue errorInit = JS_NewObject(ctx);
    JS_SetPropertyStr(ctx, errorInit, "promise", JS_DupValue(ctx, promise));
    JS_SetPropertyStr(ctx, errorInit, "reason", JS_DupValue(ctx, error));
    JSValue arguments[] = {errorType, errorInit};
    JSValue rejectEventValue = JS_CallConstructor(context->ctx(), PromiseRejectionEventValue, 2, arguments);
    if (JS_IsException(rejectEventValue)) {
      context->handleException(&rejectEventValue);
      return;
    }

    auto* rejectEvent = static_cast<EventInstance*>(JS_GetOpaque(rejectEventValue, Event::kEventClassID));
    rejectEvent->setTarget(window);
    window->dispatchEvent(rejectEvent);

    JS_FreeValue(ctx, errorType);
    JS_FreeValue(ctx, errorInit);
    JS_FreeValue(ctx, rejectEventValue);
    JS_FreeValue(ctx, PromiseRejectionEventValue);

    context->drainPendingPromiseJobs();
  }
}

void ExecutionContext::dispatchGlobalUnhandledRejectionEvent(ExecutionContext* context, JSValueConst promise, JSValueConst error) {
  // Trigger onerror event.
  dispatchGlobalErrorEvent(context, error);

  // Trigger unhandledRejection event.
  dispatchPromiseRejectionEvent("unhandledrejection", context, promise, error);
}

void ExecutionContext::dispatchGlobalRejectionHandledEvent(ExecutionContext* context, JSValue promise, JSValue error) {
  // Trigger rejectionhandled event.
  dispatchPromiseRejectionEvent("rejectionhandled", context, promise, error);
}

void ExecutionContext::promiseRejectTracker(JSContext* ctx, JSValue promise, JSValue reason, int is_handled, void* opaque) {
  auto* context = static_cast<ExecutionContext*>(JS_GetContextOpaque(ctx));
  // The unhandledrejection event is the promise-equivalent of the global error event, which is fired for uncaught exceptions.
  // Because a rejected promise could be handled after the fact, by attaching catch(onRejected) or then(onFulfilled, onRejected) to it,
  // the additional rejectionhandled event is needed to indicate that a promise which was previously rejected should no longer be considered unhandled.
  if (is_handled) {
    context->m_rejectedPromise.trackHandledPromiseRejection(context, promise, reason);
  } else {
    context->m_rejectedPromise.trackUnhandledPromiseRejection(context, promise, reason);
  }
}

DOMTimerCoordinator* ExecutionContext::timers() {
  return &m_timers;
}

std::unique_ptr<NativeString> jsValueToNativeString(JSContext* ctx, JSValue value) {
  bool isValueString = true;
  if (JS_IsNull(value)) {
    value = JS_NewString(ctx, "");
    isValueString = false;
  } else if (!JS_IsString(value)) {
    value = JS_ToString(ctx, value);
    isValueString = false;
  }

  uint32_t length;
  uint16_t* buffer = JS_ToUnicode(ctx, value, &length);
  std::unique_ptr<NativeString> ptr = std::make_unique<NativeString>();
  ptr->string = buffer;
  ptr->length = length;

  if (!isValueString) {
    JS_FreeValue(ctx, value);
  }
  return ptr;
}

void buildUICommandArgs(JSContext* ctx, JSValue key, NativeString& args_01) {
  if (!JS_IsString(key))
    return;

  uint32_t length;
  uint16_t* buffer = JS_ToUnicode(ctx, key, &length);
  args_01.string = buffer;
  args_01.length = length;
}

std::unique_ptr<NativeString> stringToNativeString(const std::string& string) {
  std::u16string utf16;
  fromUTF8(string, utf16);
  NativeString tmp{};
  tmp.string = reinterpret_cast<const uint16_t*>(utf16.c_str());
  tmp.length = utf16.size();
  return std::unique_ptr<NativeString>(tmp.clone());
}

std::unique_ptr<NativeString> atomToNativeString(JSContext* ctx, JSAtom atom) {
  JSValue stringValue = JS_AtomToString(ctx, atom);
  std::unique_ptr<NativeString> string = jsValueToNativeString(ctx, stringValue);
  JS_FreeValue(ctx, stringValue);
  return string;
}

std::string jsValueToStdString(JSContext* ctx, JSValue& value) {
  const char* cString = JS_ToCString(ctx, value);
  std::string str = std::string(cString);
  JS_FreeCString(ctx, cString);
  return str;
}

std::string jsAtomToStdString(JSContext* ctx, JSAtom atom) {
  const char* cstr = JS_AtomToCString(ctx, atom);
  std::string str = std::string(cstr);
  JS_FreeCString(ctx, cstr);
  return str;
}

// An lock free context validator.
bool isContextValid(int32_t contextId) {
  if (contextId > running_context_list)
    return false;
  return valid_contexts[contextId];
}

void arrayPushValue(JSContext* ctx, JSValue array, JSValue val) {
  JSValue pushMethod = JS_GetPropertyStr(ctx, array, "push");
  JSValue arguments[] = {val};
  JSValue result = JS_Call(ctx, pushMethod, array, 1, arguments);
  JS_FreeValue(ctx, pushMethod);
  JS_FreeValue(ctx, result);
}

void arraySpliceValue(JSContext* ctx, JSValue array, uint32_t start, uint32_t deleteCount) {
  JSValue spliceMethod = JS_GetPropertyStr(ctx, array, "splice");
  JSValue arguments[] = {JS_NewUint32(ctx, start), JS_NewUint32(ctx, deleteCount)};
  JSValue result = JS_Call(ctx, spliceMethod, array, 2, arguments);
  JS_FreeValue(ctx, spliceMethod);
  JS_FreeValue(ctx, result);
}

void arraySpliceValue(JSContext* ctx, JSValue array, uint32_t start, uint32_t deleteCount, JSValue replacedValue) {
  JSValue spliceMethod = JS_GetPropertyStr(ctx, array, "splice");
  JSValue arguments[] = {JS_NewUint32(ctx, start), JS_NewUint32(ctx, deleteCount), replacedValue};
  JSValue result = JS_Call(ctx, spliceMethod, array, 3, arguments);
  JS_FreeValue(ctx, spliceMethod);
  JS_FreeValue(ctx, result);
}

void arrayInsert(JSContext* ctx, JSValue array, uint32_t start, JSValue targetValue) {
  JSValue spliceMethod = JS_GetPropertyStr(ctx, array, "splice");
  JSValue arguments[] = {JS_NewUint32(ctx, start), JS_NewUint32(ctx, 0), targetValue};
  JSValue result = JS_Call(ctx, spliceMethod, array, 3, arguments);
  JS_FreeValue(ctx, spliceMethod);
  JS_FreeValue(ctx, result);
}

int32_t arrayGetLength(JSContext* ctx, JSValue array) {
  JSValue lenVal = JS_GetPropertyStr(ctx, array, "length");
  int32_t len;
  JS_ToInt32(ctx, &len, lenVal);
  JS_FreeValue(ctx, lenVal);
  return len;
}

int32_t arrayFindIdx(JSContext* ctx, JSValue array, JSValue target) {
  int32_t len = arrayGetLength(ctx, array);
  for (int i = 0; i < len; i++) {
    JSValue v = JS_GetPropertyUint32(ctx, array, i);
    if (JS_VALUE_GET_PTR(v) == JS_VALUE_GET_PTR(target)) {
      JS_FreeValue(ctx, v);
      return i;
    };
    JS_FreeValue(ctx, v);
  }
  return -1;
}

JSValue objectGetKeys(JSContext* ctx, JSValue obj) {
  JSValue globalObject = JS_GetGlobalObject(ctx);
  JSValue object = JS_GetPropertyStr(ctx, globalObject, "Object");
  JSValue keysFunc = JS_GetPropertyStr(ctx, object, "keys");

  JSValue result = JS_Call(ctx, keysFunc, obj, 1, &obj);

  JS_FreeValue(ctx, keysFunc);
  JS_FreeValue(ctx, object);
  JS_FreeValue(ctx, globalObject);

  return result;
}

void ExecutionContext::trace(JSRuntime* rt, JSValue val, JS_MarkFunc* mark_func) {
  m_timers.trace(rt, JS_NULL, mark_func);
}

}  // namespace kraken::binding::qjs
