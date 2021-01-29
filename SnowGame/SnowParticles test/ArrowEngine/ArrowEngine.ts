class ArrowEngine {

    private content;
    private circle: HTMLDivElement | undefined;
    private point: HTMLDivElement | undefined;
    private last: HTMLDivElement | undefined;
    private down = false;

    private pointClassName = "point";
    private circleClassName = "circle";
    private lineClassName = "line";

    constructor(content: HTMLElement) {
        this.content = content || document.body;
    }

    public Setup(pointName: string, circleName: string, lineName: string) {
        this.pointClassName = pointName;
        this.circleClassName = circleName;
        this.lineClassName = lineName;
    }

    private CreateEvent(name: string, params: Object) {
        let evt = new CustomEvent(name, {
            detail: params
        });
        document.dispatchEvent(evt);
        window.dispatchEvent(evt);
    }

    private DownEvent(e: any) {
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
        })
    }

    private MoveEvent(e: any) {
        if (this.down) {
            if (this.last) {
                let size = parseInt(this.last.style.width.replace('px', '')) / 4 + "px";
                if (this.circle != undefined) {
                    this.circle.style.width = size;
                    this.circle.style.height = size;
                }

                this.last.remove()
            }
            if (this.point != undefined) {
                this.point.style.left = e.x + "px";
                this.point.style.top = e.y + "px";
            }


            this.last = this.CreateLine(this.circle, this.point);
        }
    }

    private UpEvent() {
        this.down = false;
        if (this.circle && this.last && this.point) {
            this.CreateEvent("ArrowEngineCircleBeforeAnimate", {
                circle: this.circle,
                last: this.last,
                point: this.point
            })
            let deg = parseFloat(this.last.style.transform.replace('rotate(', '').replace('deg)', ''));
            let width = parseFloat(this.last.style.width.replace('px', ''));
            this.CreateAnimation(width, deg, this.circle)
        }
        if (this.point) {
            this.point.remove();
        }
        if (this.last) {
            this.last.remove()
        }
    }

    public Run(disableCTX = true, EnableMobileMode = true) {
        if (disableCTX) {
            document.addEventListener("contextmenu", e => e.preventDefault())
        }
        if (EnableMobileMode) {
            document.addEventListener("touchstart", (e) => {
                let evt = {
                    x: e.touches[0].clientX,
                    y: e.touches[0].clientY
                }
                this.DownEvent(evt);
            })
            document.addEventListener("touchmove", (e) => {
                let evt = {
                    x: e.touches[0].clientX,
                    y: e.touches[0].clientY
                }
                this.MoveEvent(evt);
            })
            document.addEventListener("touchcancel", () => {

            })
            document.addEventListener("touchend", () => this.UpEvent())
        }
        document.addEventListener("mousedown", e => this.DownEvent(e));
        document.addEventListener("mousemove", e => this.MoveEvent(e));
        document.addEventListener("mouseup", () => this.UpEvent());
    }

    private GetCoord(element: HTMLElement, top = true) {
        let rect = element.getBoundingClientRect();
        let off = {
            left: rect.left + window.pageXOffset - rect.width / 2,
            top: rect.top + window.pageYOffset - rect.width / 2,
            width: rect.width,
            height: rect.height
        };
        let x = off.left + off.width;
        let y = off.top + off.height;
        if (!top) {
            y = off.top;
        }
        return [x, y];
    }
    private CreateLine(element: any, element2: any) {
        let el1 = this.GetCoord(element);
        let el2 = this.GetCoord(element2, false);

        let x1 = el1[0];
        let y1 = el1[1];
        let x2 = el2[0];
        let y2 = el2[1];
        let length = Math.sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
        let cx = ((x1 + x2) / 2) - (length / 2);
        let cy = ((y1 + y2) / 2) - (1 / 2);
        let deg = Math.atan2((y1 - y2), (x1 - x2)) * (180 / Math.PI);

        let line = document.createElement("div");
        line.className = this.lineClassName;
        line.style.left = cx + "px";
        line.style.top = cy + "px";
        line.style.width = length + "px";
        line.style.transform = "rotate(" + deg + "deg)";
        this.content.appendChild(line);
        this.CreateEvent("ArrowEngineCreateLine", {
            line: {
                line,
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
        })
        return line;
    }

    private CreateAnimation(width: number, deg: number, circle: HTMLDivElement) {
        circle.classList.remove("circle")
        circle.classList.add("circle-move")
        let top = parseFloat(circle.style.top.replace('px', ''));
        let left = parseFloat(circle.style.left.replace('px', ''));
        let x = 0;
        let y = 0;
        let interval = width / 10;
        let rad = Math.PI * deg / 180;
        x = Math.cos(rad);
        y = Math.sin(rad);
        let speed = 10;

        let id = setInterval(() => {
            top += y * speed;
            left += x * speed;
            circle.style.top = top + "px"
            circle.style.left = left + "px"
            if (left > window.innerWidth || left < -window.innerWidth || top < -window.innerHeight || top > window.innerHeight) {
                clearInterval(id);
                circle.remove();
                this.CreateEvent("ArrowEngineCircleAfterAnimate", {
                    intervalId: id,
                    circleWidth: width,
                    deg: deg,
                    coord: {
                        y: top,
                        x: left
                    }
                })
            }
        }, interval)
        return 1;
    }
}