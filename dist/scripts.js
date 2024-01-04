function setChangeEventOfFileInput()
{
	const statusEl = document.getElementById('status');
	const outputEl = document.getElementById('output');
	if (window.FileList && window.File && window.FileReader)
	{
		document.getElementById('file-selector').addEventListener('change', event =>
		{
			outputEl.innerText = '';
			statusEl.textContent = '';
			const file = event.target.files[0];
			if (!file.type)
			{
				statusEl.textContent = 'Error: The File.type property does not appear to be supported on this browser.';
				return;
			}
			readFile(file, outputEl);
		}); 
	}
}
	
async function sendPOST(contentBody, filename)
{
	if(filename && filename.len > 0)
	{
		console.log("fetching... " + filename);
		const response = await fetch("localhost:8000/" + filename,
		{
			method: "POST", // *GET, POST, PUT, DELETE, etc.
			mode: "cors", // no-cors, *cors, same-origin
			cache: "no-cache", // *default, no-cache, reload, force-cache, only-if-cached
			credentials: "same-origin", // include, *same-origin, omit
			headers:
			{
				"Content-Type": "text/plain",
			},
			//redirect: "follow", // manual, *follow, error
			//referrerPolicy: "no-referrer", // no-referrer, *no-referrer-when-downgrade, origin, origin-when-cross-origin, same-origin, strict-origin, strict-origin-when-cross-origin, unsafe-url
			body: contentBody, // body data type must match "Content-Type" header
		});
		console.log(response);
	}
}

async function readFile(file, outputEl)
{
	const reader = new FileReader();
	reader.addEventListener('load', async (event) =>
	{
		console.log(event.target.result);
		outputEl.textContent = event.target.result;
		await sendPOST(event.target.result, file);
	});
	reader.readAsText(file);
}

setChangeEventOfFileInput()
