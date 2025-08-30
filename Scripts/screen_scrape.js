// ==UserScript==
// @name         florr screen scraper
// @namespace    http://tampermonkey.net/
// @version      0.1
// @description  try to take over the world!
// @author       You
// @match        *florr*
// @icon         https://www.google.com/s2/favicons?sz=64&domain=florr.io
// @grant        unsafeWindow
// @run-at       document-body
// ==/UserScript==

//load into the game, load petals you want to view, run test() in the console
//images will be displayed on the new window
const noinclude = new Set();
const canvases = [];
const octxs = {};
const paths = [''];
unsafeWindow.octxs = octxs;
unsafeWindow.paths = paths;
let id = 0;
let pathid = 0;
unsafeWindow.OffscreenCanvas = new Proxy(unsafeWindow.OffscreenCanvas, {
    construct(ta,a) {
        const canv = Reflect.construct(ta,a);
        canv.id = id;
        canvases[id] = canv;
        ++id;
        return canv;
    }
});
Path2D = new Proxy(Path2D, {
    construct(ta,a) {
        const canv = new ta();
        paths[pathid] = '';
        canv.id = pathid++;
        return canv;
    }
});
([
    "clip",
    "drawImage",
    "fill",
    "fillText",
    "measureText",
    "reset",
    "save",
    "scale",
    "setTransform",
    "stroke",
    "strokeText",
    "transform",
    "arc",
    "beginPath",
    "bezierCurveTo",
    "closePath",
    "ellipse",
    "fillRect",
    "lineTo",
    "moveTo",
    "quadraticCurveTo",
    "rect",
    "strokeRect"
]).forEach(thing => {
    unsafeWindow.OffscreenCanvasRenderingContext2D.prototype[thing] = new Proxy(unsafeWindow.OffscreenCanvasRenderingContext2D.prototype[thing], {
        apply(t,ta,a) {
            octxs[ta.canvas.id] ||= '';
            if (!((thing == 'fill' || thing == 'stroke') && a.length))
            {
                octxs[ta.canvas.id] += `rr_renderer_${thing.toLowerCase()}(renderer, ${a.toString()});\n`;
                if (thing == 'fill' || thing == 'stroke') octxs[ta.canvas.id] += `rr_renderer_set_${thing}(renderer, 0xff${ta[thing + "Style"].slice(1)});\n`;
                if (thing == 'stroke')
                {octxs[ta.canvas.id] += `rr_renderer_set_line_width(renderer, ${ta.lineWidth});\n`;}
            }
            else {
                octxs[ta.canvas.id] += "Renderer::Path path = {}\n" + paths[a[0].id];
                octxs[ta.canvas.id] += `rr_renderer_${thing.toLowerCase()}(renderer);\n`;
            }
            return t.apply(ta,a);
        }
    });
});
([
    "addPath",
    "roundRect",
    "arc",
    "arcTo",
    "bezierCurveTo",
    "closePath",
    "ellipse",
    "lineTo",
    "moveTo",
    "quadraticCurveTo",
    "rect"
]).forEach(thing => {
    Path2D.prototype[thing] = new Proxy(Path2D.prototype[thing], {
        apply(t,ta,a) {
            paths[ta.id] += `path.${thing[0].toUpperCase() + thing.slice(1)}(${a.toString()});\n`;
            return t.apply(ta,a);
        }
    });
})
const newWindow = unsafeWindow.open("", "Scraped Images", "width=1000,height=750");
let canvs = newWindow.document.getElementById('canvas');
if (!canvs)
{canvs = newWindow.document.createElement('canvas');
newWindow.document.body.appendChild(canvs);}

const cc = canvs.getContext('2d');
canvs.width = 1000; canvs.height = 1000;
unsafeWindow.test = () => {
    canvs.width = 1000;
    for (let n = 0; n < canvases.length; n++)
    {
        if (canvases[n].width == 0) continue;
        cc.drawImage(canvases[n], 50 * (n % 20), 50 * ((n - (n % 20)) / 20), 50, 50);
        cc.beginPath();
        cc.font = '10px Ubuntu';
        cc.lineWidth = 2;
        cc.fillStyle = '#ffffff';
        cc.strokeStyle = '#000000';
        cc.textAlign = 'center';
        cc.textBaseLine = 'middle';
        cc.strokeText(`${n}`, 25 + 50 * (n % 20), 25 + 50 * ((n - (n % 20)) / 20));
        cc.fillText(`${n}`, 25 + 50 * (n % 20), 25 + 50 * ((n - (n % 20)) / 20));
    }
}