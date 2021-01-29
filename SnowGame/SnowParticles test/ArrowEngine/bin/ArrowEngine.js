"use strict";
var ArrowEngine = /** @class */ (function () {
    function ArrowEngine(content) {
        this.down = false;
        this.pointClassName = "point";
        this.circleClassName = "circle";
        this.lineClassName = "line";
        this.content = content || document.body;
    }
    ArrowEngine.prototype.Setup = function (pointName, circleName, lineName) {
        this.pointClassName = pointName;
        this.circleClassName = circleName;
        this.lineClassName = lineName;
    };
    ArrowEngine.prototype.CreateEvent = function (name, params) {
        var evt = new CustomEvent(name, {
            detail: params
        });
        document.dispatchEvent(evt);
        window.dispatchEvent(evt);
    };
    ArrowEngine.prototype.DownEvent = function (e) {
        if (this.down && this.circle) {
            this.circle.remove();
            if (this.point) {
                this.point.remove();
            }
        }
        this.down = true;
        this.point = document.createElement("div");
        this.point.className = this.pointClassName;
        this.point.style.left = e.x + "px";
        this.point.style.top = e.y + "px";
        this.circle = document.createElement("div");
        this.circle.className = this.circleClassName;
        this.circle.style.left = e.x - 30 / 2 + "px";
        this.circle.style.top = e.y - 30 / 2 + "px";
        this.content.appendChild(this.circle);
        this.content.appendChild(this.point);
        this.CreateEvent("ArrowEngineAddToDocument", {
            circle: this.circle,
            point: this.point
        });
    };
    ArrowEngine.prototype.MoveEvent = function (e) {
        if (this.down) {
            if (this.last) {
                var size = parseInt(this.last.style.width.replace('px', '')) / 4 + "px";
                if (this.circle != undefined) {
                    this.circle.style.width = size;
                    this.circle.style.height = size;
                }
                this.last.remove();
            }
            if (this.point != undefined) {
                this.point.style.left = e.x + "px";
                this.point.style.top = e.y + "px";
            }
            this.last = this.CreateLine(this.circle, this.point);
        }
    };
    ArrowEngine.prototype.UpEvent = function () {
        this.down = false;
        if (this.circle && this.last && this.point) {
            this.CreateEvent("ArrowEngineCircleBeforeAnimate", {
                circle: this.circle,
                last: this.last,
                point: this.point
            });
            var deg = parseFloat(this.last.style.transform.replace('rotate(', '').replace('deg)', ''));
            var width = parseFloat(this.last.style.width.replace('px', ''));
            this.CreateAnimation(width, deg, this.circle);
        }
        if (this.point) {
            this.point.remove();
        }
        if (this.last) {
            this.last.remove();
        }
    };
    ArrowEngine.prototype.Run = function (disableCTX, EnableMobileMode) {
        var _this = this;
        if (disableCTX === void 0) { disableCTX = true; }
        if (EnableMobileMode === void 0) { EnableMobileMode = true; }
        if (disableCTX) {
            document.addEventListener("contextmenu", function (e) { return e.preventDefault(); });
        }
        if (EnableMobileMode) {
            document.addEventListener("touchstart", function (e) {
                var evt = {
                    x: e.touches[0].clientX,
                    y: e.touches[0].clientY
                };
                _this.DownEvent(evt);
            });
            document.addEventListener("touchmove", function (e) {
                var evt = {
                    x: e.touches[0].clientX,
                    y: e.touches[0].clientY
                };
                _this.MoveEvent(evt);
            });
            document.addEventListener("touchcancel", function () {
            });
            document.addEventListener("touchend", function () { return _this.UpEvent(); });
        }
        document.addEventListener("mousedown", function (e) { return _this.DownEvent(e); });
        document.addEventListener("mousemove", function (e) { return _this.MoveEvent(e); });
        document.addEventListener("mouseup", function () { return _this.UpEvent(); });
    };
    ArrowEngine.prototype.GetCoord = function (element, top) {
        if (top === void 0) { top = true; }
        var rect = element.getBoundingClientRect();
        var off = {
            left: rect.left + window.pageXOffset - rect.width / 2,
            top: rect.top + window.pageYOffset - rect.width / 2,
            width: rect.width,
            height: rect.height
        };
        var x = off.left + off.width;
        var y = off.top + off.height;
        if (!top) {
            y = off.top;
        }
        return [x, y];
    };
    ArrowEngine.prototype.CreateLine = function (element, element2) {
        var el1 = this.GetCoord(element);
        var el2 = this.GetCoord(element2, false);
        var x1 = el1[0];
        var y1 = el1[1];
        var x2 = el2[0];
        var y2 = el2[1];
        var length = Math.sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
        var cx = ((x1 + x2) / 2) - (length / 2);
        var cy = ((y1 + y2) / 2) - (1 / 2);
        var deg = Math.atan2((y1 - y2), (x1 - x2)) * (180 / Math.PI);
        var line = document.createElement("div");
        line.className = this.lineClassName;
        line.style.left = cx + "px";
        line.style.top = cy + "px";
        line.style.width = length + "px";
        line.style.transform = "rotate(" + deg + "deg)";
        this.content.appendChild(line);
        this.CreateEvent("ArrowEngineCreateLine", {
            line: {
                line: line,
                width: length,
                deg: deg,
                rad: Math.PI * deg / 180
            },
            element1: {
                element: element,
                x: x1,
                y: y1
            },
            element2: {
                element: element2,
                x: x2,
                y: y2
            },
        });
        return line;
    };
    ArrowEngine.prototype.CreateAnimation = function (width, deg, circle) {
        var _this = this;
        circle.classList.remove("circle");
        circle.classList.add("circle-move");
        var top = parseFloat(circle.style.top.replace('px', ''));
        var left = parseFloat(circle.style.left.replace('px', ''));
        var x = 0;
        var y = 0;
        var interval = width / 10;
        var rad = Math.PI * deg / 180;
        x = Math.cos(rad);
        y = Math.sin(rad);
        var speed = 10;
        var id = setInterval(function () {
            top += y * speed;
            left += x * speed;
            circle.style.top = top + "px";
            circle.style.left = left + "px";
            if (left > window.innerWidth || left < -window.innerWidth || top < -window.innerHeight || top > window.innerHeight) {
                clearInterval(id);
                circle.remove();
                _this.CreateEvent("ArrowEngineCircleAfterAnimate", {
                    intervalId: id,
                    circleWidth: width,
                    deg: deg,
                    coord: {
                        y: top,
                        x: left
                    }
                });
            }
        }, interval);
        return 1;
    };
    return ArrowEngine;
}());