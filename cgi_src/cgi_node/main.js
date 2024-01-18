console.error("Finish NodeJS script <--")
console.log("Content-Type: text/html\r");
console.log("Status: 200 OK\r");
console.log("Location: localhost:8000/\r");
console.log("Set-Cookie: sessionId=nodejs38afes7a8; Path=/; Max-Age=2592000\r");
console.log("\r");
console.log("<h1>Rendered with CGI through NODEJS</h1>\r");
console.log("<h3>This is the environment</h3>");
console.log("<details close>\r");
console.log("<summary>Environment Variables</summary>\r");
console.log("<div style=\"background-color:9900FF\">\r");
Object.keys(process.env).map(it => console.log("<p>" + it + ": " + process.env[it] + "</p>\r"));
console.log("</div>\r");
console.log("</details>\r");
console.log("<button onclick=\"document.cookie = 'sessionId=; Path=/; Expires=Thu, 01 Jan 1970 00:00:01 GMT;';\">LOGOUT</button>\r");
console.error("Start NodeJS script -->")
    