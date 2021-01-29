declare class ArrowEngine {
    private content;
    private circle;
    private point;
    private last;
    private down;
    private pointClassName;
    private circleClassName;
    private lineClassName;
    /**
     * @public
     * @constructor
     * @param {HTMLElement} content Set the content for appendChild element
     */
    constructor(content: HTMLElement);
    /**
     * @public
     * @function Setup Change default element classNames
     * @param {String} pointName Set point element className
     * @param {String} circleName Set circle element className
     * @param {String} lineName Set list element className
     */
    Setup(pointName: string, circleName: string, lineName: string): void;
    private CreateEvent;
    private DownEvent;
    private MoveEvent;
    private UpEvent;
    /**
     * @public
     * @function Run
     * @param {Boolean} disableCTX Enable context menu in the document 
     * @param {Boolean} EnableMobileMode Enable touch screen event for mobile 
     */
    Run(disableCTX?: boolean, EnableMobileMode?: boolean): void;
    private GetCoord;
    private CreateLine;
    private CreateAnimation;
}