console.error("Start NodeJS script -->")
let body = "<h1>Rendered with CGI NPH through NODEJS</h1>\r\n";
body += "<h3>This is the environment</h3>\r\n";
body += "<details close>\r\n";
body += "<summary>Environment Variables</summary>\r\n";
body += "<div style=\"background-color:9900FF\">\r\n"
body += Object.keys(process.env).map(it => ("<p>" + it + ": " + process.env[it] + "</p>")).join("\r\n");
body += "</div>\r\n";
body += ("</details>\r\n");
body += "<button onclick=\"document.cookie = 'sessionId=; Path=/; Expires=Thu, 01 Jan 1970 00:00:01 GMT;';\">LOGOUT</button>\r\n";

console.log(process.env["SERVER_PROTOCOL"] + " 200 OK\r");
console.log("Server: " + process.env["SERVER_NAME"] + "\r");
console.log("Content-Type: text/html\r");
console.log("Content-Length: " + String(body.length) + "\r");
console.log("Location: localhost:8000/\r");
console.log("Set-Cookie: sessionId=nodejs38afes7a8; Path=/; Max-Age=2592000\r");
console.log("\r");
console.log(body);
// console.log("<h1>Rendered with CGI through NODEJS</h1>");
// console.log("<div style=\"background-color:9900FF\">");
// for (i=0; i<10000000000; i++)
// {
// 	parseInt(String(i));
// }
// Object.keys(process.env).map(it => console.log("<p>" + it + ": " + process.env[it] + "</p>"));
// console.log("</div>");
console.error("Finish NodeJS script <--")
