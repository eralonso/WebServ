function setChangeEventOfFileInput()
{
	const statusEl = document.getElementById('status');
	const outputNameEl = document.getElementById('outputName');
	const outputContEl = document.getElementById('outputContent');
	if (window.FileList && window.File && window.FileReader)
	{
		document.getElementById('file-selector').addEventListener('change', event =>
		{
			statusEl.textContent = '';
			outputNameEl.textContent = '';
			outputContEl.textContent = '';
			const file = event.target.files[0];
			if (!file.type)
			{
				statusEl.textContent = 'Error: The File.type property does not appear to be supported on this browser.';
				return;
			}
			outputNameEl.textContent = file.name;
			readFile(file, outputContEl);
		}); 
	}
}
	
async function sendPOST(contentBody, file)
{
	let urlLoc = window.location.href;
	urlLoc = urlLoc.split("/").slice(0, -1).join("/");
	console.log("sendPOST... " + file.name + " at url: " + urlLoc);
	if(file)
	{
		console.log("fetching... " + file.name);
		const response = await fetch(urlLoc + "/" + file.name,
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
		console.log (event.target);
		outputEl.textContent = event.target.result;
		await sendPOST(event.target.result, file);
	});
	reader.readAsText(file);
}

const urlDisplayEl = document.getElementById('urlDisplay');
urlDisplayEl.textContent = "url of page: " + window.location.href;
setChangeEventOfFileInput()
