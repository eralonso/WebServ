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
	let evalBody = ("<body>\r\n")
	evalBody += ("<h1>NODEJS Login response</h1>\r\n");
	evalBody += ("<h3>This is the environment</h3>\r\n");
	evalBody += ("<details close>\r\n");
	evalBody += ("<summary>Environment Variables</summary>\r\n");
	evalBody += ("<div style=\"background-color:9900FF\">\r\n");
	evalBody += env;
	evalBody += ("</div>\r\n");
	evalBody += ("</details>\r\n");
	evalBody += ("<script>window.location.assign(\"./nph-main.js\")</script>\n");
	evalBody += ("</body>\r");
	return evalBody;
}

async function main()
{
	console.error("Start NodeJS script -->");
	let body = getBody();
	let value = await readFromStdin();
	console.error("Read from webserv: ");
	console.error(value);
	console.error("\n");
	let acum = ""
	if (process.env["REQUEST_METHOD"] == "POST")
	{
		acum += process.env["SERVER_PROTOCOL"] + " 200 OK\r\n"
		acum += "Server: " + process.env["SERVER_NAME"] + "\r\n"
		acum += "Content-Type: text/html; charset=utf-8\r\n";
		acum += "Content-Length: " + String(body.length) + "\r\n"
		acum += "Location: "+ process.env["SERVER_NAME"] +":" + process.env["SERVER_PORT"] + "/\r\n"
		acum += "Set-Cookie: sessionId=nodejs["+ value +"]; Path=/; Max-Age=2592000\r\n";
		acum += "\r\n"
		acum += body;
	}
	else
	{
		acum += process.env["SERVER_PROTOCOL"] + " 405 NotAllowedMethod\r\n"
		acum += "Server: " + process.env["SERVER_NAME"] + "\r\n"
		acum += "Content-Type: text/html; charset=utf-8\r\n";
		acum += "Content-Length: " + String(body.length) + "\r\n"
		acum += "Location: "+ process.env["SERVER_NAME"] +":" + process.env["SERVER_PORT"] + "/\r\n"
		acum += "\r\n"
		// acum += body;
	}
	console.log(acum);
	console.error("Written to webserv: ");
	console.error(acum);
	console.error("Finish NodeJS script <--"); 

}

main();