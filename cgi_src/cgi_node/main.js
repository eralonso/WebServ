console.log("Content-Type: text/html\r");
console.log("Status: 200 OK\r");
console.log("\r");
console.log("<h1>Rendered with CGI through NODEJS</h1>");
console.log("<div style=\"background-color:9900FF\">");
// for (i=0; i<10000000000; i++)
// {
// 	parseInt(String(i));
// }
Object.keys(process.env).map(it => console.log("<p>" + it + ": " + process.env[it] + "</p>"));
console.log("</div>");
