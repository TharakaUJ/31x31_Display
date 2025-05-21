#pragma once

const char *root = R"html(
  <!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 Game Controller</title>
    <style>
        :root {
            --primary: #6200ee;
            --primary-dark: #3700b3;
            --secondary: #03dac6;
            --secondary-dark: #018786;
            --danger: #cf6679;
            --danger-dark: #b00020;
            --background: #121212;
            --surface: #1e1e1e;
            --on-background: rgba(255, 255, 255, 0.87);
            --on-surface: rgba(255, 255, 255, 0.87);
            --elevated: #2d2d2d;
        }
        
        * {
            box-sizing: border-box;
            margin: 0;
            padding: 0;
        }
        
        body {
            display: flex;
            flex-direction: column;
            align-items: center;
            justify-content: center;
            min-height: 100vh;
            background-color: var(--background);
            margin: 0;
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            color: var(--on-background);
            padding: 20px;
        }
        
        .controller-container {
            width: 100%;
            max-width: 500px;
        }
        
        .controller {
            background-color: var(--surface);
            border-radius: 24px;
            padding: 30px;
            box-shadow: 0 10px 30px rgba(0, 0, 0, 0.5);
            display: flex;
            flex-direction: column;
            align-items: center;
            gap: 30px;
            width: 100%;
            position: relative;
            overflow: hidden;
        }
        
        .controller::before {
            content: "";
            position: absolute;
            top: 0;
            left: 0;
            right: 0;
            height: 4px;
            background: linear-gradient(90deg, var(--primary), var(--secondary));
        }
        
        .title {
            text-align: center;
            width: 100%;
        }
        
        .title h1 {
            margin: 0;
            font-size: 28px;
            font-weight: 600;
            background: linear-gradient(90deg, var(--primary), var(--secondary));
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
            letter-spacing: 0.5px;
        }
        
        .title p {
            margin-top: 5px;
            font-size: 14px;
            opacity: 0.7;
        }
        
        .d-pad {
            display: grid;
            grid-template-columns: repeat(3, 90px);
            grid-template-rows: repeat(3, 90px);
            gap: 10px;
        }
        
        .d-pad button {
            width: 100%;
            height: 100%;
            font-size: 18px;
            border-radius: 18px;
            background-color: var(--elevated);
            color: var(--on-surface);
            border: none;
            cursor: pointer;
            transition: all 0.2s cubic-bezier(0.4, 0, 0.2, 1);
            display: flex;
            align-items: center;
            justify-content: center;
            position: relative;
            overflow: hidden;
        }
        
        .d-pad button:active {
            background-color: var(--primary-dark);
        }
        
        .d-pad button:hover:not(.empty) {
            background-color: var(--primary);
            box-shadow: 0 5px 15px rgba(98, 0, 238, 0.3);
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
        
        .d-pad button svg {
            filter: drop-shadow(0 2px 3px rgba(0, 0, 0, 0.2));
        }
        
        .center-btn {
            background: linear-gradient(145deg, var(--primary), var(--primary-dark)) !important;
        }
        
        .center-btn:hover {
            background: linear-gradient(145deg, var(--primary-dark), var(--primary)) !important;
        }
        
        .menu-buttons {
            display: flex;
            justify-content: center;
            gap: 20px;
            width: 100%;
        }
        
        .menu-btn {
            flex: 1;
            max-width: 130px;
            height: 55px;
            border-radius: 50px;
            background-color: var(--elevated);
            color: var(--on-surface);
            border: none;
            font-size: 16px;
            font-weight: 600;
            letter-spacing: 1px;
            cursor: pointer;
            transition: all 0.2s cubic-bezier(0.4, 0, 0.2, 1);
            text-transform: uppercase;
        }
        
        .menu-btn:active {
            background-color: var(--elevated);
        }
        
        .select-btn {
            background: linear-gradient(145deg, var(--secondary), var(--secondary-dark));
            color: #000;
        }
        
        .select-btn:hover {
            background: linear-gradient(145deg, var(--secondary-dark), var(--secondary));
            box-shadow: 0 5px 15px rgba(3, 218, 198, 0.3);
        }
        
        .back-btn {
            background: linear-gradient(145deg, var(--danger), var(--danger-dark));
        }
        
        .back-btn:hover {
            background: linear-gradient(145deg, var(--danger-dark), var(--danger));
            box-shadow: 0 5px 15px rgba(207, 102, 121, 0.3);
        }
        
        .feedback {
            margin-top: 10px;
            padding: 15px;
            height: 50px;
            width: 100%;
            background-color: var(--elevated);
            border-radius: 50px;
            text-align: center;
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 16px;
            transition: all 0.3s ease;
            position: relative;
        }
        
        .status-indicator {
            position: absolute;
            left: 15px;
            width: 12px;
            height: 12px;
            border-radius: 50%;
            background-color: var(--secondary);
            box-shadow: 0 0 10px var(--secondary);
        }
        
        .status-indicator.sending {
            background-color: var(--primary);
            animation: pulse 1s infinite;
        }
        
        .status-indicator.error {
            background-color: var(--danger);
        }
        
        @keyframes pulse {
            0% {
                box-shadow: 0 0 0 0 rgba(98, 0, 238, 0.7);
            }
            
            70% {
                box-shadow: 0 0 0 10px rgba(98, 0, 238, 0);
            }
            
            100% {
                box-shadow: 0 0 0 0 rgba(98, 0, 238, 0);
            }
        }
        
        .button-press {
            position: absolute;
            background: radial-gradient(circle, rgba(255,255,255,0.3) 0%, rgba(255,255,255,0) 70%);
            border-radius: 50%;
            pointer-events: none;
            transform: translate(-50%, -50%);
            animation: ripple 0.8s linear;
        }
        
        @keyframes ripple {
            to {
                width: 300px;
                height: 300px;
                opacity: 0;
            }
        }
        
        @media (max-width: 500px) {
            .d-pad {
                grid-template-columns: repeat(3, 70px);
                grid-template-rows: repeat(3, 70px);
                gap: 8px;
            }
            
            .menu-btn {
                height: 50px;
                font-size: 14px;
            }
            
            .title h1 {
                font-size: 24px;
            }
        }
    </style>
</head>
<body>
    <div class="controller-container">
        <div class="controller">
            <div class="title">
                <h1>ESP32 Game Controller</h1>
                <p>Wireless Game Control Interface</p>
            </div>
            
            <div class="d-pad">
                <button class="empty"></button>
                <button onclick="sendRequest('/up')" class="direction-btn">
                    <svg width="40" height="40" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round">
                        <path d="M12 19V5M12 5L5 12M12 5L19 12"/>
                    </svg>
                </button>
                <button class="empty"></button>
                
                <button onclick="sendRequest('/left')" class="direction-btn">
                    <svg width="40" height="40" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round">
                        <path d="M19 12H5M5 12L12 5M5 12L12 19"/>
                    </svg>
                </button>
                <button onclick="sendRequest('/center')" class="center-btn">
                    <svg width="32" height="32" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round">
                        <circle cx="12" cy="12" r="4"/>
                        <path d="M12 2v2"/>
                        <path d="M12 20v2"/>
                        <path d="M2 12h2"/>
                        <path d="M20 12h2"/>
                    </svg>
                </button>
                <button onclick="sendRequest('/right')" class="direction-btn">
                    <svg width="40" height="40" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round">
                        <path d="M5 12H19M19 12L12 5M19 12L12 19"/>
                    </svg>
                </button>
                
                <button class="empty"></button>
                <button onclick="sendRequest('/down')" class="direction-btn">
                    <svg width="40" height="40" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5" stroke-linecap="round" stroke-linejoin="round">
                        <path d="M12 5V19M12 19L5 12M12 19L19 12"/>
                    </svg>
                </button>
                <button class="empty"></button>
            </div>
            
            <div class="menu-buttons">
                <button class="menu-btn select-btn" onclick="sendRequest('/select')">Select</button>
                <button class="menu-btn back-btn" onclick="sendRequest('/back')">Back</button>
            </div>
            
            <div class="feedback" id="feedback">
                <div class="status-indicator" id="status-indicator"></div>
                <span id="feedback-text">Ready</span>
            </div>
        </div>
    </div>
    
    <script>
        // Create visual feedback without DOM manipulation
        document.querySelectorAll('button:not(.empty)').forEach(button => {
            button.addEventListener('click', function(e) {
                // Change background color briefly instead of adding ripple element
                const originalBackground = this.style.backgroundColor;
                if (!this.classList.contains('empty')) {
                    this.style.backgroundColor = 'rgba(255, 255, 255, 0.2)';
                    setTimeout(() => {
                        this.style.backgroundColor = originalBackground;
                    }, 200);
                }
            });
        });
        
        function sendRequest(endpoint) {
            const feedbackText = document.getElementById('feedback-text');
            const statusIndicator = document.getElementById('status-indicator');
            const buttonName = endpoint.replace('/', '').toUpperCase();
            
            // Update UI state
            feedbackText.textContent = `Sending ${buttonName} command...`;
            statusIndicator.className = 'status-indicator sending';
            
            fetch(endpoint)
                .then(response => {
                    if (response.ok) {
                        feedbackText.textContent = `${buttonName} command sent`;
                        statusIndicator.className = 'status-indicator';
                        console.log(endpoint + " request successful");
                    } else {
                        feedbackText.textContent = `${buttonName} command failed`;
                        statusIndicator.className = 'status-indicator error';
                        console.error(endpoint + " request failed");
                    }
                })
                .catch(error => {
                    feedbackText.textContent = `Error sending ${buttonName} command`;
                    statusIndicator.className = 'status-indicator error';
                    console.error("Error:", error);
                });
                
            // Reset status after delay
            setTimeout(() => {
                feedbackText.textContent = "Ready";
                statusIndicator.className = 'status-indicator';
            }, 3000);
        }
    </script>
</body>
</html>
)html";

const char *simulate = R"html(
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