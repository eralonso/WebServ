console.log("<h1>Rendered with CGI through NODEJS</h1>");
console.log("<div style=\"background-color:9900FF\">");
Object.keys(process.env).map(it => console.log("<p>" + it + ": " + process.env[it] + "</p>"));
console.log("</div>");
