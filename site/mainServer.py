import os
from flask import Flask, render_template, request, redirect, url_for, send_from_directory
from werkzeug import secure_filename

# Initialize the Flask application
app = Flask(__name__)

# This is the path to the upload directory
app.config['UPLOAD_FOLDER'] = '../RPi'

# Allowed extensions
app.config['ALLOWED_EXTENSIONS'] = set(['png'])

def allowed_file(filename):
    return '.' in filename and \
           filename.rsplit('.', 1)[1] in app.config['ALLOWED_EXTENSIONS']

# Index
@app.route('/')
def index():
    return render_template('index.html')

# File upload
@app.route('/upload', methods=['POST'])
def upload():
    # Get the name of the uploaded file
    file = request.files['file']
    # Check if the file is one of the allowed types/extensions
    if file and allowed_file(file.filename):

        # Delete old files
        filename = "image0.png"
        oldPNG = "../RPi/image0.png"
        oldBMP = "../RPi/image0.bmp"
        if os.path.isfile(oldPNG):
            os.remove(oldPNG)
        if os.path.isfile(oldBMP):
            print("Found file")
            os.remove(oldBMP)

        # Save file
        file.save(os.path.join(app.config['UPLOAD_FOLDER'], filename))
        os.system("cd ../RPi; python png2bmp.py")

        # Redirect after upload
        return redirect(url_for('uploaded_file',
                                filename=filename))

# After upload
@app.route('/uploads/<filename>')
def uploaded_file(filename):
    os.system("cd ../RPi; sudo ./microPainter")
    return send_from_directory(app.config['UPLOAD_FOLDER'],
                               filename)

if __name__ == "__main__":
    app.run(host= '172.28.70.209', port=9000)
