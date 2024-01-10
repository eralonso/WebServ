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

function setChangeEventOfDelFileInput()
{
	const fileEditEl = document.getElementById('del-file-selector');
	const outputNameEl = document.getElementById('outputDelName');
	if (window.FileList && window.File && window.FileReader)
	{
		document.getElementById('delSubmit').addEventListener('click', event =>
		{
			file = fileEditEl.value;
			outputNameEl.textContent = file;
			sendDELETE(file);
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
			method: "POST",
			mode: "cors",
			cache: "no-cache",
			credentials: "same-origin",
			headers:
			{
				"Content-Type": "text/plain",
			},
			body: contentBody,
		});
		console.log(response);
	}
}

async function sendDELETE(file)
{
	let urlLoc = window.location.href;
	urlLoc = urlLoc.split("/").slice(0, -2);
	console.log(urlLoc);
	urlLoc.push("files_upload")
	console.log(urlLoc);
	urlLoc = urlLoc.join("/");
	console.log(urlLoc);
	console.log("sendDELETE... " + file + " at url: " + urlLoc);
	if(file)
	{
		console.log("fetching... " + file);
		const response = await fetch(urlLoc + "/" + file,
		{
			method: "DELETE",
			mode: "cors",
			cache: "no-cache",
			credentials: "same-origin",
			headers:
			{
				"Content-Type": "text/plain",
			},
			body: "",
		});
		console.log(response);
	}
}

async function readFile(file, outputEl)
{
	const reader = new FileReader();
	const dec = new TextDecoder("utf-8");
	reader.addEventListener('load', async (event) =>
	{
		console.log(event.target.result);
		console.log(event.target.result.length);
		console.log (event.target);
		outputEl.textContent = dec.decode(event.target.result);
		await sendPOST(event.target.result, file);
	});
	reader.readAsArrayBuffer(file);
}

const urlDisplayEl = document.getElementById('urlDisplay');
urlDisplayEl.textContent = "url of page: " + window.location.href;
setChangeEventOfFileInput();
setChangeEventOfDelFileInput();
