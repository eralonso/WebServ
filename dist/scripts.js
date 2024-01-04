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
			const reader = new FileReader();
			reader.addEventListener('load', event => 
			{
				outputEl.textContent = event.target.result;
				console.log(result);
			});
			// reader.readAsDataURL(file);
			reader.readAsText(file);
			
		}); 
	}
}
	
async function sendPOST()
{
	const el = document.getElementById("filename");
	const filename = el.value;
	if(filename && filename.len() > 0)
	{

		const response = await fetch(url,
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
			body: data, // body data type must match "Content-Type" header
		});
	}
}

function readImage(file)
{
	const reader = new FileReader();
	reader.addEventListener('load', (event) =>
	{
		img.src = event.target.result;
	});
	reader.readAsText(file);
}
