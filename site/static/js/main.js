$(function() {
    // Drag and drop file uploads
		var myDropzone = new Dropzone('body', {
			url: "/upload",
			autoProcessQueue: true
		});

		// File upload progress
	   	myDropzone.on("totaluploadprogress", function(progress) {

			// Round the progress value
			progress = Math.round(progress);
			if (progress == 100 && !(iconHidden)){

				iconHidden = true;

				setTimeout(function () {
					// Hiding progress bar then show loading icon.
					hide("progress-bar");
		      		hide("progress-shape");
				}, 500)

				show("loading-icon");
			}

			// Update the value of the progress bar
			$("#progress-bar")
				.css("width", progress + "%")
				.attr("aria-valuenow", progress)
				.text(progress + "%");

			$('.roller').width(progress + '%');
	    });

		myDropzone.on('addedfile', function(file) {
			// $('#submit-file').show();
			// $('#submit-file').click(function() {
			// 	myDropzone.processQueue();
			// })
		fileFailed = false;
		myDropzone.isValidFile;

		});

		// Sending dropped file
		myDropzone.on('sending', function(file, xhr, formData) {
	      	$('.meter').show();

	      	if (!(initialized)){
				show("progress-bar");
		      	show("progress-shape");
		      	initialized = true;
    	      	}

}
