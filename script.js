var lines = new Array(128);
var line_count = 0;

function drawLine(ctx){

    for(var n = 0; n < line_count * 4; n += 4)
    {
        ctx.beginPath();
        ctx.moveTo(lines[n] + 10, lines[n+1]);
        ctx.lineTo(lines[n+2] + 10, lines[n+3] + 20);
        ctx.stroke();
    }
}

function appendLine(x_pre, y_pre, x_new, y_new){

    lines[line_count * 4] = x_pre
    lines[line_count * 4 + 1] = y_pre
    lines[line_count * 4 + 2] = x_new
    lines[line_count * 4 + 3] = y_new
    line_count++;
}
