from flask import Flask, render_template, url_for, flash, redirect, request
#from forms import WaypointsForm
from serverfile import webme

app = Flask(__name__)


server = webme()
@app.route("/",methods=['GET', 'POST'])
@app.route("/home",methods=['GET', 'POST'])
def home():
        #form = WaypointsForm()

        #if form.validate_on_submit():
        #flash(f'Coordinates Sucessfuly Received', 'success')
        if request.method=='POST':
            if request.form['submit_button'] == 'Submit':
                lo1 = request.form['latitude1']
                la1 = request.form['longitude1']
                lo2 = request.form['latitude2']
                la2 = request.form['longitude2']
                coordinates = [lo1, la1, lo2, la2]
                server.send(coordinates)
                #print(coordinates)
                print(lo1)
                print(la1)
                print(lo2)
                print(la2)
                if request.method == 'POST':
                    return redirect(url_for('home'))
        return render_template('home.html')

@app.route('/stop', methods = ['POST'])
def stop():
        stopvalue="J"
        print("STOP")
        server.send(stopvalue)
        return redirect('/')

if __name__ == '__main__':
    app.run(debug=True)
