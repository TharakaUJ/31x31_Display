#pragma once

const char* root = R"html(
  <!DOCTYPE html>
<html>
<head>
    <title>ESP32 Game Controller</title>
    <style>
        body {
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            height: 100vh;
            background-color: #2c3e50;
            margin: 0;
            font-family: Arial, sans-serif;
            color: white;
        }
        
        .controller {
            background-color: #34495e;
            border-radius: 20px;
            padding: 30px;
            box-shadow: 0 10px 20px rgba(0, 0, 0, 0.5);
            display: flex;
            flex-direction: column;
            align-items: center;
            gap: 20px;
        }
        
        .title {
            margin-bottom: 20px;
            text-align: center;
            color: #ecf0f1;
            text-shadow: 0 2px 4px rgba(0, 0, 0, 0.3);
        }
        
        .d-pad {
            display: grid;
            grid-template-columns: repeat(3, 80px);
            grid-template-rows: repeat(3, 80px);
            gap: 5px;
        }
        
        .d-pad button {
            width: 100%;
            height: 100%;
            font-size: 18px;
            border-radius: 10px;
            background-color: #3498db;
            color: white;
            border: none;
            cursor: pointer;
            transition: all 0.2s ease;
            display: flex;
            align-items: center;
            justify-content: center;
        }
        
        .d-pad button:active {
            transform: scale(0.95);
            background-color: #2980b9;
            box-shadow: inset 0 0 10px rgba(0, 0, 0, 0.3);
        }
        
        .d-pad button:hover {
            background-color: #2980b9;
        }
        
        .d-pad button.empty {
            background-color: transparent;
            box-shadow: none;
            cursor: default;
        }
        
        .d-pad button.empty:active, .d-pad button.empty:hover {
            transform: none;
            background-color: transparent;
        }
        
        .menu-buttons {
            display: flex;
            justify-content: center;
            gap: 40px;
            margin-top: 20px;
        }
        
        .menu-btn {
            width: 120px;
            height: 50px;
            border-radius: 25px;
            background-color: #e74c3c;
            color: white;
            border: none;
            font-size: 16px;
            cursor: pointer;
            transition: all 0.2s ease;
        }
        
        .menu-btn:active {
            transform: scale(0.95);
            background-color: #c0392b;
            box-shadow: inset 0 0 10px rgba(0, 0, 0, 0.3);
        }
        
        .menu-btn:hover {
            background-color: #c0392b;
        }
        
        .select-btn {
            background-color: #2ecc71;
        }
        
        .select-btn:hover, .select-btn:active {
            background-color: #27ae60;
        }
        
        .feedback {
            margin-top: 20px;
            padding: 10px;
            height: 30px;
            background-color: rgba(0, 0, 0, 0.2);
            border-radius: 10px;
            text-align: center;
            transition: all 0.3s ease;
        }
    </style>
</head>
<body>
    <div class="controller">
        <div class="title">
            <h1>ESP32 Game Controller</h1>
        </div>
        
        <div class="d-pad">
            <button class="empty"></button>
            <button onclick="sendRequest('/up')">
                <svg width="40" height="40" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                    <path d="M12 19V5M12 5L5 12M12 5L19 12"/>
                </svg>
            </button>
            <button class="empty"></button>
            
            <button onclick="sendRequest('/left')">
                <svg width="40" height="40" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                    <path d="M19 12H5M5 12L12 5M5 12L12 19"/>
                </svg>
            </button>
            <button onclick="sendRequest('/center')">
                <svg width="30" height="30" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                    <circle cx="12" cy="12" r="3"/>
                </svg>
            </button>
            <button onclick="sendRequest('/right')">
                <svg width="40" height="40" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                    <path d="M5 12H19M19 12L12 5M19 12L12 19"/>
                </svg>
            </button>
            
            <button class="empty"></button>
            <button onclick="sendRequest('/down')">
                <svg width="40" height="40" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2">
                    <path d="M12 5V19M12 19L5 12M12 19L19 12"/>
                </svg>
            </button>
            <button class="empty"></button>
        </div>
        
        <div class="menu-buttons">
            <button class="menu-btn select-btn" onclick="sendRequest('/select')">SELECT</button>
            <button class="menu-btn" onclick="sendRequest('/back')">BACK</button>
        </div>
        
        <div class="feedback" id="feedback">Ready</div>
    </div>
    
    <script>
        function sendRequest(endpoint) {
            const feedback = document.getElementById('feedback');
            const buttonName = endpoint.replace('/', '').toUpperCase();
            
            feedback.textContent = `Sending ${buttonName} command...`;
            
            fetch(endpoint)
                .then(response => {
                    if (response.ok) {
                        feedback.textContent = `${buttonName} command sent successfully`;
                        console.log(endpoint + " request successful");
                    } else {
                        feedback.textContent = `${buttonName} command failed`;
                        console.error(endpoint + " request failed");
                    }
                })
                .catch(error => {
                    feedback.textContent = `Error sending ${buttonName} command`;
                    console.error("Error:", error);
                });
                
            // Visual feedback for button press
            setTimeout(() => {
                feedback.textContent = "Ready";
            }, 2000);
        }
    </script>
</body>
</html>
)html";

const char* simulate = R"html(
<!DOCTYPE html>
<html>
<head>
    <title>LED Simulator</title>
    <style>
        canvas { border: 1px solid black; }
    </style>
</head>
<body>
    <h1>LED Matrix Simulation</h1>
    <canvas id="matrix" width="620" height="620"></canvas>

    <script>
        const canvas = document.getElementById('matrix');
        const ctx = canvas.getContext('2d');
        const cellSize = 20;

        function draw() {
            fetch('/framebuffer')
                .then(res => res.json())
                .then(data => {
                    ctx.clearRect(0, 0, canvas.width, canvas.height);
                    data.forEach(pixel => {
                        ctx.fillStyle = `rgb(${pixel.r},${pixel.g},${pixel.b})`;
                        ctx.fillRect(pixel.x * cellSize, pixel.y * cellSize, cellSize, cellSize);
                    });
                });
        }

        setInterval(draw, 100); // 10 FPS
    </script>
</body>
</html>
)html";