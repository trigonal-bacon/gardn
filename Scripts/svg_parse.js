const parse_path = (path, x = 0, y = 0) => {
    let at = 0;
    path = path.replaceAll(/[A-Za-z]/g, str => ' ' + str + ' ').replaceAll(",", " ").replaceAll("-", " -");
    path = path.slice(1, path.length - 1).split(' ').filter(x => x);
    path = path.map(x => {
        x = x.split(".");
        if (x.length == 1) return x;
        if (x.length == 2) return x[0] + "." + x[1];
        let first = x.shift();
        x = x.map(y => "0." + y);
        x[0] = first + x[0].slice(1);
        return x;
    }).flat();
    let ret_str = 'ctx.begin_path();\n';
    let curr_op = 'm';
    const bx = x, by = y;
    const op_parse = _ => {
        switch(curr_op)
        {
            case 'M':
                x = parseFloat(path[at++]) + bx;
                y = parseFloat(path[at++]) + by;
                ret_str += `ctx.move_to(${x.toFixed(2)}, ${y.toFixed(2)});\n`;
                break;
            case 'L':
            {
                const prev_x = x;
                const prev_y = y;
                x = parseFloat(path[at++]) + bx;
                y = parseFloat(path[at++]) + by;
                if (Math.abs(x - prev_x) > 0.001 || Math.abs(y - prev_y) > 0.001)
                    ret_str += `ctx.line_to(${x.toFixed(2)}, ${y.toFixed(2)});\n`;
                break;
            }
            case 'H':
            {
                const prev_x = x;
                x = parseFloat(path[at++]) + bx;
                if (Math.abs(x - prev_x) > 0.001)
                    ret_str += `ctx.line_to(${x.toFixed(2)}, ${y.toFixed(2)});\n`;
                break;
            }
            case 'V':
            {
                const prev_y = y;
                y = parseFloat(path[at++]) + by;
                if (Math.abs(y - prev_y) > 0.001)
                    ret_str += `ctx.line_to(${x.toFixed(2)}, ${y.toFixed(2)});\n`;
                break;
            }
            case 'Q':
            {
                const x1 = parseFloat(path[at++]) + bx;
                const y1 = parseFloat(path[at++]) + by;
                x = parseFloat(path[at++]) + bx;
                y = parseFloat(path[at++]) + by;
                ret_str += `ctx.qcurve_to(${x1.toFixed(2)}, ${y1.toFixed(2)}, ${x.toFixed(2)}, ${y.toFixed(2)});\n`;
                break;
            }
            case 'C':
            {
                const x1 = parseFloat(path[at++]) + bx;
                const y1 = parseFloat(path[at++]) + by;
                const x2 = parseFloat(path[at++]) + bx;
                const y2 = parseFloat(path[at++]) + by;
                x = parseFloat(path[at++]) + bx;
                y = parseFloat(path[at++]) + by;
                ret_str += `ctx.bcurve_to(${x1.toFixed(2)}, ${y1.toFixed(2)}, ${x2.toFixed(2)}, ${y2.toFixed(2)}, ${x.toFixed(2)}, ${y.toFixed(2)});\n`;
                break;
            }
            case 'm':
                x += parseFloat(path[at++]);
                y += parseFloat(path[at++]);
                ret_str += `ctx.move_to(${x.toFixed(2)}, ${y.toFixed(2)});\n`;
                break;
            case 'l':
            {
                const prev_x = x;
                const prev_y = y;
                x += parseFloat(path[at++]);
                y += parseFloat(path[at++]);
                if (Math.abs(x - prev_x) > 0.001 || Math.abs(y - prev_y) > 0.001)
                    ret_str += `ctx.line_to(${x.toFixed(2)}, ${y.toFixed(2)});\n`;
                break;
            }
            case 'h':
            {
                const prev_x = x;
                x += parseFloat(path[at++]);
                if (Math.abs(x - prev_x) > 0.001)
                    ret_str += `ctx.line_to(${x.toFixed(2)}, ${y.toFixed(2)});\n`;
                break;
            }
            case 'v':
            {
                const prev_y = y;
                y += parseFloat(path[at++]);
                if (Math.abs(y - prev_y) > 0.001)
                    ret_str += `ctx.line_to(${x.toFixed(2)}, ${y.toFixed(2)});\n`;
                break;
            }
            case 'q':
            {
                const x1 = x + parseFloat(path[at++]);
                const y1 = y + parseFloat(path[at++]);
                x += parseFloat(path[at++]);
                y += parseFloat(path[at++]);
                ret_str += `ctx.qcurve_to(${x1.toFixed(2)}, ${y1.toFixed(2)}, ${x.toFixed(2)}, ${y.toFixed(2)});\n`;
                break;
            }
            case 'c':
            {
                const x1 = x + parseFloat(path[at++]);
                const y1 = y + parseFloat(path[at++]);
                const x2 = x + parseFloat(path[at++]);
                const y2 = y + parseFloat(path[at++]);
                x += parseFloat(path[at++]);
                y += parseFloat(path[at++]);
                ret_str += `ctx.bcurve_to(${x1.toFixed(2)}, ${y1.toFixed(2)}, ${x2.toFixed(2)}, ${y2.toFixed(2)}, ${x.toFixed(2)}, ${y.toFixed(2)});\n`;
                break;
            }
            case 'z':
                console.log("uh");
                break;
            default:
                throw new Error('uh oh: ' + path[at] + ' ' + path.slice(at - 10, at + 10));
        }
    }
    while (at < path.length)
    {
        if (!Number.isFinite(parseFloat(path[at])))    
            curr_op = path[at++];
        console.log(at, path.length);
        op_parse();
    }
    return ret_str;
}

const parse_svg = str => {
    let doc = new DOMParser().parseFromString(str, 'text/xml');
    const [offset_x, offset_y] = doc.getElementsByTagName('svg')[0].attributes.viewBox.value.split(' ').map(x => parseFloat(x)).slice(2);
    doc = [...doc.getElementsByTagName('path')].map(x => x.attributes).filter(x => !x["clip-rule"] && (!x["fill-opacity"] || parseFloat(x["fill-opacity"].nodeValue) !== 0));
    let ret = "";
    for (let x of doc)
    {
        if (x["fill"])
        {
            if (x["fill-opacity"])
                ret += `rr_renderer_set_fill(renderer, 0x${((x["fill-opacity"].nodeValue * 255) | 0).toString(16).padStart(2, '0')}${x["fill"].nodeValue.slice(1)});\n`;
            else
                ret += `rr_renderer_set_fill(renderer, 0xff${x["fill"].nodeValue.slice(1)});\n`;
        }
        ret += parse_path(x["d"].nodeValue, -offset_x / 2, -offset_y / 2);
        if (x["fill"])
            ret += `rr_renderer_fill(renderer);\n`;
    }
    console.log(ret);
}

const transform_path = str => str.replaceAll(");","").replaceAll("path.MoveTo(","M").replaceAll("path.BezierCurveTo(","C").replaceAll("path.QuadraticCurveTo(","Q").replaceAll("path.LineTo(","L").replaceAll('\n', ' ').replaceAll("path.ClosePath(", "");

console.log(parse_path(`M0-10 C5.523-10 10-5.41 10 .253 10 4.782 7.138 8.624 3.167 9.981 2.66 10.082 2.48 9.762 2.48 9.489 2.48 9.151 2.492 8.047 2.492 6.675 2.492 5.719 2.172 5.095 1.813 4.777 4.04 4.523 6.38 3.656 6.38-.282 6.38-1.402 5.992-2.316 5.35-3.034 5.454-3.293 5.797-4.336 5.252-5.748 5.252-5.748 4.414-6.023 2.505-4.697 1.706-4.924.85-5.038 0-5.042-.85-5.038-1.705-4.924-2.503-4.697-4.414-6.023-5.254-5.748-5.254-5.748-5.797-4.336-5.454-3.293-5.351-3.034-5.99-2.316-6.381-1.402-6.381-.282-6.381 3.646-4.046 4.526-1.825 4.785-2.111 5.041-2.37 5.493-2.46 6.156-3.03 6.418-4.478 6.871-5.37 5.304-5.37 5.304-5.899 4.319-6.903 4.247-6.903 4.247-7.878 4.234-6.971 4.87-6.971 4.87-6.316 5.185-5.861 6.37-5.861 6.37-5.274 8.2-2.492 7.58-2.487 8.437-2.478 9.245-2.478 9.489-2.478 9.76-2.662 10.077-3.161 9.982-7.135 8.627-10 4.783-10 .253-10-5.41-5.522-10 0-10`));