const colors = require("colors");


module.exports = {
    log:(title, msg)=> 
    {
        console.log(colors.bgBlue(colors.black(title)) + " " + msg);
    },
    error:(title, msg) =>
    {
        console.log(colors.bgRed(colors.black(title)) + " " + msg);
    },
    warm:(title, msg) =>
    {
        console.log(colors.bgYellow(colors.black(title)) + " " + msg);
    },
    win:(title,msg)=>{
        console.log(colors.bgGreen(colors.black(title)) + " " + msg);
    }
}