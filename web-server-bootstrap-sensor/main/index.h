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
            $('#cp4').colorpicker({hexNumberSignPrefix: false}).on('changeColor', function(e) {
                document.getElementById('choosenPalette').style.color = e.color.toString('rgba');
                setPalette(e.color.toString());                
            });
        });
        $(function() {
            $('#demo').colorpicker();
        });
                
        // Change LED Status
        function setLED(led) {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById('ledState').innerHTML = this.responseText;
                }
            };
            xhttp.open('GET', 'setLED?ledState='+led, true);
            xhttp.send();
        }

        // Change Color Palette Led Strip
        function setPalette(palette) {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function() {
                if (this.readyState == 4 && this.status == 200) {
                    document.getElementById('choosenPalette').innerHTML = this.responseText;
                }
            };
            xhttp.open('GET', 'changePalette?palette='+palette, true);
            xhttp.send();
        }

        // Call a function repetatively with 2 Second interval
        /*
        setInterval(function() {
            getData("T");
            getData("H");
        }, 2000); //2000mSeconds update rate
        */

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
    <title>Control Station</title>
</head>
<body>
    <div class='container-fluid'>
        <div class='row'>
            <div class='col-md-12'>        
                <span><h3>Controle da Fita de LED</h3><span>
                <div class='row'>
                    <div class='col-md-4'>
                        <h4 class='text-left'>Palette Choosen
                            <span class='badge'><td><span id='choosenPalette'>NA</span></td></span>
                        </h4>
                    </div>
                </div>
                <div class='row'>
                    <div class='col-md-12'>
                        <div class="btn-group" role="group">
                            <button type='button' onclick='setPalette("cloud")' class='btn btn-default'>
                                <span class="glyphicon glyphicon-cloud" aria-hidden="true"></span>
                                Cloud
                            </button>
                            <button type='button' onclick='setPalette("lava")' class='btn btn-default'>
                                <span class="glyphicon glyphicon-fire" aria-hidden="true"></span>
                                Lava
                            </button>
                            <button type='button' onclick='setPalette("ocean")' class='btn btn-default'>
                                <span class="glyphicon glyphicon-tint" aria-hidden="true"></span>
                                Ocean
                            </button>
                            <button type='button' onclick='setPalettsetPalettee("forest")' class='btn btn-default'>
                                <span class="glyphicon glyphicon-leaf" aria-hidden="true"></span>
                                Forest
                            </button>
                            <button type='button' onclick='setPalette("rainbow")' class='btn btn-default'>
                                <span class="glyphicon glyphicon-eye-open" aria-hidden="true"></span>
                                Rainbow
                            </button>
                            <button type='button' onclick='setPalette("rainbowStriped")' class='btn btn-default'>
                                <span class="glyphicon glyphicon-eye-close" aria-hidden="true"></span>
                                RainbowStriped
                            </button>
                            <button type='button' onclick='setPalette("party")' class='btn btn-default'>
                                <span class="glyphicon glyphicon-music" aria-hidden="true"></span>
                                Party
                            </button>
                            <button type='button' onclick='setPalette("heat")' class='btn btn-default'>
                                <span class="glyphicon glyphicon-warning-sign" aria-hidden="true"></span>
                                Heat
                            </button>
                            <button type='button' onclick='setPalette("blink")' class='btn btn-default'>
                                <span class="glyphicon glyphicon-info-sign" aria-hidden="true"></span>
                                Blink
                            </button>                            
                            <button type='button' onclick='setPalette("random")' class='btn btn-default'>
                                <span class="glyphicon glyphicon-star" aria-hidden="true"></span>
                                Random
                            </button>
                            <button type='button' onclick='setPalette("shutdown")' class='btn btn-default'>
                                <span class="glyphicon glyphicon-pause" aria-hidden="true"></span>
                                Shutdown
                            </button>
                        </div>
                    </div>
                </div>
            </div>
            <div class='row'>
                <div class='col-md-12 text-center' style="margin-top: 5px;">
                    <a href="#" class="btn btn-warning" id="cp4">
                        <span class="glyphicon glyphicon-wrench" aria-hidden="true"></span>
                        Pick color
                    </a>
                </div>
            </div>
        </div>        
        <div class='row'>
            <div class='col-md-12'>
                <span><h3>Controle de medi&ccedil;&atilde;o de temperatura/umidade</h3><span>
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
            </div>
        </div>
        <div class='row'>
            <div class='col-md-12'>
                <h3>Controle de LED</h3>
                <div class='row'>
                    <div class='col-md-4'>
                        <h4 class='text-left'>LED
                            <span class='badge'><td><span id='ledState'>NA</span></td></span>
                        </h4>
                    </div>
                    <div class='col-md-1'>
                        <button type='button' onclick='setLED(1)' class='btn btn-success btn-sm'>ON</button>
                    </div>
                    <div class='col-md-1'>
                        <button type='button' onclick='setLED(0)' class='btn btn-danger btn-sm'>OFF</button>
                    </div>
                </div>
            </div>
        </div>
        <!-- <iframe src="https://giphy.com/embed/21GCae4djDWtP5soiY" width="384" height="480" frameBorder="0" class="giphy-embed" allowFullScreen></iframe><p><a href="https://giphy.com/gifs/reaction-dog-omg-21GCae4djDWtP5soiY">via GIPHY</a></p> -->
    </div>
</body>

</html>
)=====";
