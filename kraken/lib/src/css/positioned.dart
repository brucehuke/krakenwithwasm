import 'package:flutter/rendering.dart';
import 'package:kraken/dom.dart';
import 'package:kraken/rendering.dart';
import 'package:kraken/css.dart';

// CSS Positioned Layout: https://drafts.csswg.org/css-position/

enum CSSPositionType {
  static,
  relative,
  absolute,
  fixed,
  sticky,
}

Offset _getRelativeOffset(CSSStyleDeclaration style) {
  CSSPositionType position = CSSPositionedLayout.parsePositionType(style[POSITION]);
  if (position == CSSPositionType.relative) {
    double dx;
    double dy;

    // @TODO support auto value
    if (style.contains(LEFT) && style[LEFT] != AUTO) {
      dx = CSSLength.toDisplayPortValue(style[LEFT]);
    } else if (style.contains(RIGHT) && style[RIGHT] != AUTO) {
      var _dx = CSSLength.toDisplayPortValue(style[RIGHT]);
      if (_dx != null) dx = -_dx;
    }

    if (style.contains(TOP) && style[TOP] != AUTO) {
      dy = CSSLength.toDisplayPortValue(style[TOP]);
    } else if (style.contains(BOTTOM) && style[BOTTOM] != AUTO) {
      var _dy = CSSLength.toDisplayPortValue(style[BOTTOM]);
      if (_dy != null) dy = -_dy;
    }

    if (dx != null || dy != null) {
      return Offset(dx ?? 0, dy ?? 0);
    }
  }
  return null;
}

BoxSizeType _getChildWidthSizeType(RenderBox child) {
  if (child is RenderTextBox) {
    return child.widthSizeType;
  } else if (child is RenderBoxModel) {
    return child.widthSizeType;
  }
  return null;
}

BoxSizeType _getChildHeightSizeType(RenderBox child) {
  if (child is RenderTextBox) {
    return child.heightSizeType;
  } else if (child is RenderBoxModel) {
    return child.heightSizeType;
  }
  return null;
}

// RenderPositionHolder may be affected by overflow: scroller offset.
// We need to reset these offset to keep positioned elements render at their original position.
Offset _getRenderPositionHolderScrollOffset(RenderPositionHolder holder, RenderObject root) {
  RenderBoxModel parent = holder.parent;
  while (parent != null && parent != root) {
    if (parent.clipX || parent.clipY) {
      return Offset(parent.scrollLeft, parent.scrollTop);
    }
    parent = parent.parent;
  }
  return null;
}

// Margin auto has special rules for positioned element
// which will override the default position rule
// https://www.w3.org/TR/CSS21/visudet.html#abs-non-replaced-width
Offset _getAutoMarginPositionedElementOffset(double x, double y, RenderBox child, Size parentSize) {
  if (child is RenderBoxModel) {
    CSSStyleDeclaration childStyle = child.style;
    String marginLeft = childStyle[MARGIN_LEFT];
    String marginRight = childStyle[MARGIN_RIGHT];
    String marginTop = childStyle[MARGIN_TOP];
    String marginBottom = childStyle[MARGIN_BOTTOM];
    String width = childStyle[WIDTH];
    String height = childStyle[HEIGHT];
    String left = childStyle[LEFT];
    String right = childStyle[RIGHT];
    String top = childStyle[TOP];
    String bottom = childStyle[BOTTOM];

    // 'left' + 'margin-left' + 'border-left-width' + 'padding-left' + 'width' + 'padding-right'
    // + 'border-right-width' + 'margin-right' + 'right' = width of containing block
    if ((left.isNotEmpty && left != AUTO) &&
        (right.isNotEmpty && right != AUTO) &&
        (width.isNotEmpty && width != AUTO)) {
      if (marginLeft == AUTO) {
        double leftValue = CSSLength.toDisplayPortValue(left) ?? 0.0;
        double rightValue = CSSLength.toDisplayPortValue(right) ?? 0.0;
        double remainingSpace = parentSize.width - child.boxSize.width - leftValue - rightValue;

        if (marginRight == AUTO) {
          x = leftValue + remainingSpace / 2;
        } else {
          x = leftValue + remainingSpace;
        }
      }
    }

    if ((top.isNotEmpty && top != AUTO) &&
        (bottom.isNotEmpty && bottom != AUTO) &&
        (height.isNotEmpty && height != AUTO)) {
      if (marginTop == AUTO) {
        double topValue = CSSLength.toDisplayPortValue(top) ?? 0.0;
        double bottomValue = CSSLength.toDisplayPortValue(bottom) ?? 0.0;
        double remainingSpace = parentSize.height - child.boxSize.height - topValue - bottomValue;

        if (marginBottom == AUTO) {
          y = topValue + remainingSpace / 2;
        } else {
          y = topValue + remainingSpace;
        }
      }
    }
  }
  return Offset(x ?? 0, y ?? 0);
}

/// Check whether render object parent is layout.
bool _isLayout(RenderObject renderer, { RenderObject ancestor }) {
  if (renderer == null || !renderer.attached) {
    return false;
  }

  while (renderer != null && renderer != ancestor) {
    if (renderer is RenderBox) {
      // Whether this render box has undergone layout and has a [size].
      if (!renderer.hasSize) {
        return false;
      }
    } else if (renderer is RenderSliver) {
      // The geometry of a sliver should be set only during the sliver's
      // [performLayout] or [performResize] functions.
      if (renderer.geometry == null) {
        return false;
      }
    }

    renderer = renderer.parent;
  }

  return true;
}

class CSSPositionedLayout {
  static CSSPositionType parsePositionType(String input) {
    switch (input) {
      case RELATIVE:
        return CSSPositionType.relative;
      case ABSOLUTE:
        return CSSPositionType.absolute;
      case FIXED:
        return CSSPositionType.fixed;
      case STICKY:
        return CSSPositionType.sticky;
    }
    return CSSPositionType.static;
  }

  static RenderLayoutParentData getPositionParentData(CSSStyleDeclaration style, RenderLayoutParentData parentData) {
    CSSPositionType positionType = CSSPositionedLayout.parsePositionType(style[POSITION]);
    parentData.position = positionType;

    if (style.contains(TOP)) {
      parentData.top = CSSLength.toDisplayPortValue(style[TOP]);
    }
    if (style.contains(LEFT)) {
      parentData.left = CSSLength.toDisplayPortValue(style[LEFT]);
    }
    if (style.contains(BOTTOM)) {
      parentData.bottom = CSSLength.toDisplayPortValue(style[BOTTOM]);
    }
    if (style.contains(RIGHT)) {
      parentData.right = CSSLength.toDisplayPortValue(style[RIGHT]);
    }
    parentData.width = CSSLength.toDisplayPortValue(style[WIDTH]) ?? 0;
    parentData.height = CSSLength.toDisplayPortValue(style[HEIGHT]) ?? 0;
    parentData.zIndex = CSSLength.toInt(style[Z_INDEX]) ?? 0;

    parentData.isPositioned = positionType == CSSPositionType.absolute || positionType == CSSPositionType.fixed;

    return parentData;
  }

  static void applyRelativeOffset(Offset relativeOffset, RenderBox renderBox, CSSStyleDeclaration style) {
    RenderLayoutParentData boxParentData = renderBox?.parentData;

    // Don't set offset if it was already set
    if (boxParentData.isOffsetSet) {
      return;
    }

    if (boxParentData != null) {
      Offset styleOffset;
      // Text node does not have relative offset
      if (renderBox is! RenderTextBox && style != null) {
        styleOffset = _getRelativeOffset(style);
      }

      if (relativeOffset != null) {
        if (styleOffset != null) {
          boxParentData.offset = relativeOffset.translate(styleOffset.dx, styleOffset.dy);
        } else {
          boxParentData.offset = relativeOffset;
        }
      } else {
        boxParentData.offset = styleOffset;
      }
    }
  }

  static void layoutPositionedChild(Element parentElement, RenderBox parent, RenderBox child) {
    RenderBoxModel parentRenderBoxModel = parentElement.renderBoxModel;
    final RenderLayoutParentData childParentData = child.parentData;

    // Default to no constraints. (0 - infinite)
    BoxConstraints childConstraints = const BoxConstraints();
    Size trySize = parentRenderBoxModel.contentConstraints.biggest;
    Size parentSize = trySize.isInfinite ? parentRenderBoxModel.contentConstraints.smallest : trySize;

    BoxSizeType widthType = _getChildWidthSizeType(child);
    BoxSizeType heightType = _getChildHeightSizeType(child);

    // If child has no width, calculate width by left and right.
    // Element with intrinsic size such as image will not stretch
    if (childParentData.width == 0.0 &&
        widthType != BoxSizeType.intrinsic &&
        childParentData.left != null &&
        childParentData.right != null) {
      childConstraints = childConstraints.tighten(width: parentSize.width - childParentData.left - childParentData.right);
    }
    // If child has not height, should be calculate height by top and bottom
    if (childParentData.height == 0.0 &&
        heightType != BoxSizeType.intrinsic &&
        childParentData.top != null &&
        childParentData.bottom != null) {
      childConstraints =
          childConstraints.tighten(height: parentSize.height - childParentData.top - childParentData.bottom);
    }
    // Should create relayoutBoundary for positioned child.
    child.layout(childConstraints, parentUsesSize: false);
  }

  static void applyPositionedChildOffset(RenderBoxModel parent, RenderBoxModel child, Size parentSize, EdgeInsets borderEdge) {
    final RenderLayoutParentData childParentData = child.parentData;
    // Calc x,y by parentData.
    double x, y;

    double childMarginTop = 0;
    double childMarginBottom = 0;
    double childMarginLeft = 0;
    double childMarginRight = 0;

    Element childEl = parent.elementManager.getEventTargetByTargetId<Element>(child.targetId);
    RenderBoxModel childRenderBoxModel = childEl.renderBoxModel;
    childMarginTop = childRenderBoxModel.marginTop;
    childMarginBottom = childRenderBoxModel.marginBottom;
    childMarginLeft = childRenderBoxModel.marginLeft;
    childMarginRight = childRenderBoxModel.marginRight;

    // Offset to global coordinate system of base.
    if (childParentData.position == CSSPositionType.absolute || childParentData.position == CSSPositionType.fixed) {
      RenderObject root = parent.elementManager.getRootRenderObject();
      Offset positionHolderScrollOffset = _getRenderPositionHolderScrollOffset(childRenderBoxModel.renderPositionHolder, parent) ?? Offset.zero;

      // If [renderPositionHolder] is not laid out, then base offset must be [Offset.zero].
      Offset baseOffset = _isLayout(childRenderBoxModel.renderPositionHolder, ancestor: root) ?
          (childRenderBoxModel.renderPositionHolder.localToGlobal(positionHolderScrollOffset, ancestor: root) -
            parent.localToGlobal(Offset(parent.scrollLeft, parent.scrollTop), ancestor: root))
        : Offset.zero;

      double borderLeft = borderEdge != null ? borderEdge.left : 0;
      double borderRight = borderEdge != null ? borderEdge.right : 0;
      double borderTop = borderEdge != null ? borderEdge.top : 0;
      double borderBottom = borderEdge != null ? borderEdge.bottom : 0;

      double top = childParentData.top != null ?
        childParentData.top + borderTop + childMarginTop : baseOffset.dy + childMarginTop;
      if (childParentData.top == null && childParentData.bottom != null) {
        top = parentSize.height - child.boxSize.height - borderBottom - childMarginBottom -
          ((childParentData.bottom) ?? 0);
      }

      double left = childParentData.left != null ?
        childParentData.left + borderLeft + childMarginLeft : baseOffset.dx + childMarginLeft;
      if (childParentData.left == null && childParentData.right != null) {
        left = parentSize.width - child.boxSize.width - borderRight - childMarginRight -
          ((childParentData.right) ?? 0);
      }

      x = left;
      y = top;
    }

    Offset offset = _getAutoMarginPositionedElementOffset(x, y, child, parentSize);
    childParentData.offset = offset;
  }
}