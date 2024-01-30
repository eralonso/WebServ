function readFromStdin()
{
	let stdin = process.stdin;
	let inputChunks = [];
  
	stdin.resume();
	stdin.setEncoding('utf8');
 	stdin.on('data', 
		function (chunk)
		{
			inputChunks.push(chunk);
		});
  
	return new Promise((resolve, reject) =>
	{
		stdin.on('end',
			function ()
			{
				let readFromStdin = inputChunks.join('');
				resolve(readFromStdin);
			})
		stdin.on('error',
			function ()
			{
				reject(Error('error during read'));
			})
		stdin.on('timeout',
			function ()
			{
				reject(Error('timout during read'));
			})
	})
}

function getBody()
{
	let env = Object.keys(process.env).map(it => ("<p>" + it + ": " + process.env[it] + "</p>\r")).join("\n");
	let evalBody = ("<body>\r\n");
	evalBody += ("<h1>Rendered with CGI through NODEJS</h1>\r\n");
	evalBody += ("<h3>This is the environment</h3>\r\n");
	evalBody += ("<details close>\r\n");
	evalBody += ("<summary>Environment Variables</summary>\r\n");
	evalBody += ("<div style=\"background-color:9900FF\">\r\n");
	evalBody += env;
	evalBody += ("</div>\r\n");
	evalBody += ("</details>\r\n");
	let userCookie = process.env["HTTP_COOKIE"];
	let userAr = userCookie.split("[");
	userCookie = userAr[userAr.length-1].split("]")[0];
	userAr = userCookie.split("&");
	userCookie = userAr[0];
	userAr = userCookie.split("=");
	userCookie = userAr[userAr.length-1];
	if (userCookie != "" )
	{
		evalBody += "<h3>Current User: " + userCookie + "</h3>\r\n";
		evalBody += "<button onclick=\"document.cookie = 'sessionId=; Path=/; Expires=Thu, 01 Jan 1970 00:00:01 GMT;'; location.reload();\">LOGOUT</button>\r\n";
	}
	else
	{
		evalBody += "<form method=\"POST\" action=\"/cgi_src/node_cgi/loginAction.js\" style=\"display: flex; flex-direction: column; max-width:20em; margin: auto; padding: 1em; border: solid black 1px\">\r\n";
		evalBody += "<h3>Login Form</h3>\r\n";
		evalBody += "<div style=\"max-width:18em; margin: auto; padding: 1em;\">\r\n";
		evalBody += "<label for=\"user\">user</label>\r\n";
		evalBody += "<input type=\"user\" id=\"user\" name=\"user\"\>\r\n";
		evalBody += "</div>\r\n";
		evalBody += "<div style=\"max-width:18em; margin: auto; padding: 1em;\">\r\n";
		evalBody += "<label for=\"pass\">password</label>\r\n";
		evalBody += "<input type=\"password\" id=\"pass\" name=\"pass\"\>\r\n";
		evalBody += "</div>\r\n";
		evalBody += "<input type=\"submit\" style=\"width:16em; self-justify: center; margin: auto; padding: 1em;\" id=\"submit\"\>\r\n";
		evalBody += "</form>\r\n";
	}
	evalBody += ("</body>\r");
	return evalBody;
}

async function main()
{
	// console.error("Start NodeJS script -->")
	let body = getBody();
	let value = await readFromStdin();
	// console.error("Read from webserv: ");
	// console.error(value);
	// console.error("\n");
	
	let acum = ""
	acum += ("Content-Type: text/html\r\n");
	acum += ("Status: 200 OK\r\n");
	acum += ("\r\n");
	acum += body;
	
	console.log(acum);
	// console.error("Written to webserv: ");
	// console.error(acum);
	// console.error("Finish NodeJS script <--")
}

main();