// The entry file of your WebAssembly module.

import {document} from './htmlexport'
import {HTMLCanvasElement,CanvasRenderingContext2D} from './elements/index'

@external("env", "add_native")
declare function add_native(a: i32,b:i32): i32

var text1 = document.createTextNode("hello Kraken!!");
  var br = document.createElement("br");
  var text2= document.createTextNode("你好 北海!");

  var p = document.createElement("p");
  p.setAttribute("class","p");
  p.setAttribute("style","textAlign:center");

  p.appendChild(text1);
  p.appendChild(br);
  p.appendChild(text2);

  document.body!.appendChild(p);

  var c=document.getElementById("myCanvas") as HTMLCanvasElement;
  var ctx=c.getContext<CanvasRenderingContext2D>("2d");
  ctx!.setfillStyle("#FF00FF");
  ctx!.fillRect(0,0,150,75);

export function add(a: i32, b: i32): i32 {
  var ret = a+b;
  //
  /*var text1 = document.createTextNode('hello Kraken!!');
  var br = document.createElement('br');
  var text2= document.createTextNode('你好 北海!');

  var p = document.createElement('p');
  p.setAttribute('class','p');
  p.setAttribute('style','textAlign:center');

  p.appendChild(text1);
  p.appendChild(br);
  p.appendChild(text2);

  document.body!.appendChild(p);*/

  return ret;
}

export function addf(a: f64, b: f64): f64 {
  var ret = a+b;
  return ret;
}

export function add2(a: i32, b: i32,c:i32): i32 {
  var ret = a+b+c;
  return ret ;
}
