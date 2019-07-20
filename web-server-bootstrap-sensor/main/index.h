const char MAIN_page[] = R"=====(
<!DOCTYPE HTML>
<html lang='pt-br'>

<head>
    <meta http-equiv='refresh' name='viewport' content='width=device-width, initial-scale=1' />
    <link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/twitter-bootstrap/3.3.7/css/bootstrap.min.css'>
    <link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/bootstrap-colorpicker/2.5.3/css/bootstrap-colorpicker.min.css'>
    <script src='https://cdnjs.cloudflare.com/ajax/libs/jquery/3.3.1/jquery.min.js'></script>
    <script src='https://cdnjs.cloudflare.com/ajax/libs/twitter-bootstrap/3.3.7/js/bootstrap.min.js'></script>
    <script src='https://cdnjs.cloudflare.com/ajax/libs/bootstrap-colorpicker/2.5.3/js/bootstrap-colorpicker.min.js'></script>
    <script>
        $(function() {
            $('#cp4').colorpicker().on('changeColor', function(e) {
                $('body')[0].style.backgroundColor = e.color.toString('rgba');
            });
        });
        $(function() {
            $('#demo').colorpicker();
        });
        function sendData(led) {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById('ledState').innerHTML = this.responseText;
                }
            };
            xhttp.open('GET', 'setLED?ledState='+led, true);
            xhttp.send();
        }
        
        // Call a function repetatively with 2 Second interval
        setInterval(function() {            
            getData("T");
            getData("H");
        }, 2000); //2000mSeconds update rate
        
        function getData(sensor) {
            var xhttp = new XMLHttpRequest();
            let idSensor = sensor === 'T' ? 'temp' : 'humidity';
            let sensorMethod = sensor === 'T' ? 'readTemp' : 'readHum';
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById(idSensor).innerHTML = this.responseText;
                }
            };
            xhttp.open('GET', sensorMethod, true);
            xhttp.send();
        }
    </script>
    <title>Esta&ccedil;&atilde;o de medi&ccedil;&atilde;o</title>
</head>
<body>
    <div class='container-fluid'>
        <div class='row'>
            <div class='col-md-12'>
                <span class='bifs'><h3>Esta&ccedil;&atilde;o de medi&ccedil;&atilde;o de temperatura/umidade</h3><span>
                <table class='table'>
                    <thead>
                        <tr>
                            <th>Sensor</th>
                            <th>Medida</th>
                            <th>Valor</th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr>
                            <td>DHT22</td>
                            <td>Temperatura</td>
                            <td><span id='temp'>0&deg;C</span></td>
                        </tr>
                        <tr class='active'>
                            <td>DHT22</td>
                            <td>Umidade</td>
                            <td><span id='humidity'>0%</span></td>
                        </tr>
                    </tbody>
                </table>
                <h3>GPIO</h3>
                <div class='row'>
                    <div class='col-md-4'>
                        <h4 class='text-left'>LED 
                            <span class='badge'><td><span id='ledState'>NA</span></td></span>
                        </h4>
                    </div>
                    <div class='col-md-4'>
                        <button type='button' onclick='sendData(1)' class='btn btn-success btn-lg'>ON</button>
                    </div>
                    <div class='col-md-4'>                        
                        <button type='button' onclick='sendData(0)' class='btn btn-danger btn-lg'>OFF</button>
                    </div>
                </div>
                <a href="#" class="btn btn-default" id="cp4">Change background color</a>
            </div>
        </div>
    </div>
</body>

</html>
)=====";