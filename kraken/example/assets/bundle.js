/*var text1 = document.createTextNode('Hello Kraken!');
var br = document.createElement('br');
var text2 = document.createTextNode('你好，北海！');
var p = document.createElement('p');
p.className = 'p';
p.style.textAlign = 'center';
p.appendChild(text1);
p.appendChild(br);
p.appendChild(text2);

document.body.appendChild(p);

var c=document.getElementById("myCanvas");
var ctx=c.getContext("2d");
ctx.fillStyle="#FF00FF";
ctx.fillRect(0,0,150,75);



var b = add2(10,20,8);
console.log("js call wasm add2:" + b);*/

var a = add(10,20);
console.log("js call wasm add:" + a);

//var c = addf(9.8,12.3);
//console.log("js call wasm addf:" + c);

