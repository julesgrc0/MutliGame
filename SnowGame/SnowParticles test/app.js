// let down = false;
// let X;
// let Y;
// let circle;
// let point;
// let last;

class ArrowEngine {

    content;
    circle;
    point;
    last;
    X = 0;
    Y = 0;
    down = false;

    pointClassName = "point";
    circleClassName = "circle";
    lineClassName = "line";

    constructor(content) {
        this.content = content || document.body;
    }
    Setup(pointName, circleName, lineName) {
        this.pointClassName = pointName;
        this.circleClassName = circleName;
        this.lineClassName = lineName;
    }

    CreateEvent(name,params){
        let evt = new CustomEvent(name, {
            detail: params
        });
        document.dispatchEvent(evt);
        window.dispatchEvent(evt);
    }

    CancelEvent(){
        if (this.down && this.circle) {
            this.down = false;
            this.circle.remove();
            if (this.point) {
                this.point.remove();
            
            }
            if(this.last){
                this.last.remove();
            }
        }
    }

    DownEvent(e){
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
        this.X = e.x - 30 / 2;
        this.Y = e.y - 30 / 2;
        this.circle.style.left = e.x - 30 / 2 + "px";
        this.circle.style.top = e.y - 30 / 2 + "px";

        this.content.appendChild(this.circle);
        this.content.appendChild(this.point);
        this.CreateEvent("ArrowEngineAddToDocument",{
            circle:this.circle,
            point:this.point
        })
    }

    MoveEvent(e){
        if (this.down) {
            if (this.last) {
                let size = parseInt(this.last.style.width.replace('px', '')) / 4 + "px";
                this.circle.style.width = size;
                this.circle.style.height = size;
                this.last.remove()
            }
            this.point.style.left = e.x + "px";
            this.point.style.top = e.y + "px";

            this.last = this.CreateLine(this.circle, this.point);
        }
    }

    UpEvent(){
            this.down = false;
            if (this.circle && this.last && this.point) {
                this.CreateEvent("ArrowEngineCircleBeforeAnimate",{
                    circle:this.circle,
                    last:this.last,
                    point:this.point
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

    Run(disableCTX = true,EnableMobileMode = true) {
        if (disableCTX) {
            document.addEventListener("contextmenu", e => e.preventDefault())
        }
        if(EnableMobileMode){
            document.addEventListener("touchstart",(e)=>{
                let evt = {
                    x:e.touches[0].clientX,
                    y:e.touches[0].clientY
                }
                this.DownEvent(evt);
            })
            document.addEventListener("touchmove",(e)=>{
                let evt = {
                    x:e.touches[0].clientX,
                    y:e.touches[0].clientY
                }
                this.MoveEvent(evt);
            })
            document.addEventListener("touchcancel",()=>{

            })
            document.addEventListener("touchend",()=>this.UpEvent())
        }
        document.addEventListener("mousedown", e => this.DownEvent(e));
        document.addEventListener("mousemove", e => this.MoveEvent(e));
        document.addEventListener("mouseup",()=>this.UpEvent());
    }

    GetCoord(element, top = true) {
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
    CreateLine(element, element2) {
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
        this.CreateEvent("ArrowEngineCreateLine",{
            line:{
                line,
                width:length,
                deg:deg,
                rad:Math.PI * deg / 180
            },
            element1:{
                element:element,
                x:x1,
                y:y1
            },
            element2:{
                element:element2,
                x:x2,
                y:y2
            },
        })
        return line;
    }

    CreateAnimation(width, deg, circle) {
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
                this.CreateEvent("ArrowEngineCircleAfterAnimate",{
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

new ArrowEngine(document.body).Run();

document.addEventListener("ArrowEngineCreateLine",e=>{
    console.log(e.detail.line.deg+"°");
})


// function GetCoord(element, top = true) {
//     let rect = element.getBoundingClientRect();
//     let off = {
//         left: rect.left + window.pageXOffset - rect.width / 2,
//         top: rect.top + window.pageYOffset - rect.width / 2,
//         width: rect.width,
//         height: rect.height
//     };
//     let x = off.left + off.width;
//     let y = off.top + off.height;
//     if (!top) {
//         y = off.top;
//     }
//     return [x, y];
// }

// function CreateLine(circleElement, mousePoint) {
//     let circleCoord = GetCoord(circleElement);
//     let mousePCoord = GetCoord(mousePoint, false);
//     let x1 = circleCoord[0];
//     let y1 = circleCoord[1];
//     let x2 = mousePCoord[0];
//     let y2 = mousePCoord[1];
//     let length = Math.sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
//     let cx = ((x1 + x2) / 2) - (length / 2);
//     let cy = ((y1 + y2) / 2) - (1 / 2);
//     let deg = Math.atan2((y1 - y2), (x1 - x2)) * (180 / Math.PI);
//     let line = document.createElement("div");
//     line.className = "line";
//     line.style.left = cx + "px";
//     line.style.top = cy + "px";
//     line.style.width = length + "px";
//     line.style.transform = "rotate(" + deg + "deg)";
//     document.body.appendChild(line);
//     return line;
// }

// function CreateAnimation(size, width, deg, circle) {
//     circle.classList.remove("circle")
//     circle.classList.add("circle-move")
//     let top = parseFloat(circle.style.top.replace('px', ''));
//     let left = parseFloat(circle.style.left.replace('px', ''));
//     let x = 0;
//     let y = 0;
//     let interval = width / 10;
//     let rad = Math.PI * deg / 180;
//     x = Math.cos(rad); 
//     y = Math.sin(rad);
//     let speed = 10;

//     let id = setInterval(() => {
//         top += y * speed;
//         left += x * speed;
//         circle.style.top = top + "px"
//         circle.style.left = left + "px"
//         if(left>window.innerWidth||left< -window.innerWidth||top< -window.innerHeight||top>window.innerHeight){
//             clearInterval(id);
//             circle.remove();
//             console.log("remove");
//         }
//         console.log(left,window.innerWidth,top,window.innerHeight);
//     }, interval)
// }

// document.addEventListener("mousedown", (e) => {
//     if(down && circle){
//         circle.remove();
//         if(point){
//             point.remove();
//         }
//     }
//     down = true;
//     point = document.createElement("div");
//     circle = document.createElement("div");

//     point.className = "point";
//     point.style.left = e.x + "px";
//     point.style.top = e.y + "px";

//     circle.className = "circle";
//     X = e.x - 30 / 2;
//     Y = e.y - 30 / 2;
//     circle.style.left = e.x - 30 / 2 + "px";
//     circle.style.top = e.y - 30 / 2 + "px";

//     document.body.appendChild(circle);
//     document.body.appendChild(point);
// });

// document.addEventListener("mousemove", (e) => {
//     if (down) {
//         if (last) {
//             let size = parseInt(last.style.width.replace('px', '')) / 4 + "px";
//             circle.style.width = size;
//             circle.style.height = size;
//             last.remove()
//         }
//         point.style.left = e.x + "px";
//         point.style.top = e.y + "px";

//         last = CreateLine(circle, point);
//     }
// });

// document.addEventListener("contextmenu",e=>e.preventDefault())

// document.addEventListener("mouseup", () => {
//     down = false;
//     if (circle && last && point) {
//         let deg = parseFloat(last.style.transform.replace('rotate(', '').replace('deg)', ''));
//         let size = parseFloat(circle.style.height.replace('px', ''));
//         let width = parseFloat(last.style.width.replace('px', ''));
//         CreateAnimation(size, width, deg, circle)
//     }
//     if (point) {
//         point.remove();
//     }
//     if (last) {
//         last.remove()
//     }
// });