/*
 * Copyright (C) 2019-present Alibaba Inc. All rights reserved.
 * Author: Kraken Team.
 */

import 'package:kraken/dom.dart';

const String DIV = 'DIV';

class DivElement extends Element {
  DivElement(int targetId, ElementManager elementManager) : super(targetId, elementManager, tagName: DIV);
}