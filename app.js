document.addEventListener('DOMContentLoaded', function() {
    // Initialize terminal
    const term = new Terminal({
        cursorBlink: true,
        fontSize: 14,
        fontFamily: 'Menlo, Monaco, "Courier New", monospace',
        theme: {
            background: '#121212',
            foreground: '#F8F8F8',
            cursor: '#F8F8F8',
            selection: 'rgba(255, 255, 255, 0.3)'
        }
    });
    
    const fitAddon = new FitAddon.FitAddon();
    term.loadAddon(fitAddon);
    
    // Attach terminal to DOM
    term.open(document.getElementById('terminal'));
    fitAddon.fit();
    
    const statusIndicator = document.getElementById('status-indicator');
    const clearBtn = document.getElementById('clear-btn');
    const restartBtn = document.getElementById('restart-btn');
    
    // WebSocket setup
    let socket = null;
    let connected = false;
    let processRunning = false;
    
    function connectWebSocket() {
        // Use secure WebSocket in production, fallback to non-secure for local development
        const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
        const wsUrl = `${protocol}//${window.location.host}/terminal`;
        
        socket = new WebSocket(wsUrl);
        
        socket.onopen = () => {
            connected = true;
            statusIndicator.textContent = 'Connected';
            statusIndicator.classList.add('connected');
            term.clear();
            term.writeln('Connected to terminal service');
            term.writeln('Starting Library Management System...\n');
            startProcess();
        };
        
        socket.onmessage = (event) => {
            const data = JSON.parse(event.data);
            if (data.type === 'output') {
                term.write(data.content);
            } else if (data.type === 'status') {
                if (data.status === 'started') {
                    processRunning = true;
                } else if (data.status === 'exited') {
                    processRunning = false;
                    term.writeln('\r\nProcess exited. You can restart the application.');
                }
            }
        };
        
        socket.onclose = () => {
            connected = false;
            processRunning = false;
            statusIndicator.textContent = 'Disconnected';
            statusIndicator.classList.remove('connected');
            term.writeln('\r\nConnection closed. Attempting to reconnect in 3 seconds...');
            setTimeout(connectWebSocket, 3000);
        };
        
        socket.onerror = (error) => {
            console.error('WebSocket error:', error);
            term.writeln('\r\nWebSocket error occurred. Please check console for details.');
        };
    }
    
    // Initialize the terminal UI with simulated output
    function initializeTerminalUI() {
        term.clear();
        term.writeln('\x1b[1;35m╔════════════════════════════════════════╗\x1b[0m');
        term.writeln('\x1b[1;35m║               Library Management System               ║\x1b[0m');
        term.writeln('\x1b[1;35m╚════════════════════════════════════════╝\x1b[0m\n');
        
        term.writeln('\x1b[1;33m  ╔──────────╗\x1b[0m');
        term.writeln('\x1b[1;33m  ║ 1. Login ║\x1b[0m');
        term.writeln('\x1b[1;33m  ╚──────────╝\x1b[0m');
        term.writeln('\x1b[2m  ╔──────────╝\x1b[0m\n');
        
        term.writeln('\x1b[1;35m  ╔─────────╗\x1b[0m');
        term.writeln('\x1b[1;35m  ║ 2. Exit ║\x1b[0m');
        term.writeln('\x1b[1;35m  ╚─────────╝\x1b[0m');
        term.writeln('\x1b[2m  ╔─────────╝\x1b[0m\n');
        
        term.write('\x1b[35mChoice: \x1b[0m');
    }
    
    // Since we don't have an actual WebSocket server, we'll simulate one with terminal output
    function simulateOutput() {
        initializeTerminalUI();
        
        // Set up input handling for simulation
        let inputBuffer = '';
        let inputState = 'main';
        
        term.onData(e => {
            if (e === '\r') { // Enter key
                term.writeln('');
                processInput(inputBuffer);
                inputBuffer = '';
            } else if (e === '\u007F') { // Backspace
                if (inputBuffer.length > 0) {
                    inputBuffer = inputBuffer.slice(0, -1);
                    term.write('\b \b');
                }
            } else if (e >= ' ' && e <= '~') { // Printable characters
                inputBuffer += e;
                term.write(e);
            }
        });
        
        function processInput(input) {
            switch (inputState) {
                case 'main':
                    if (input === '1') {
                        term.writeln('Email: ');
                        inputState = 'email';
                    } else if (input === '2') {
                        term.writeln('Exiting application...');
                        setTimeout(() => {
                            term.clear();
                            term.writeln('Library Management System has been closed.');
                            term.writeln('Click "Restart Application" to start again.');
                        }, 1000);
                    } else {
                        term.writeln('Invalid choice! Please enter 1 or 2.');
                        term.write('\x1b[35mChoice: \x1b[0m');
                    }
                    break;
                    
                case 'email':
                    // Store email for later verification
                    const email = input;
                    term.writeln('Password: ');
                    inputState = 'password';
                    break;
                    
                case 'password':
                    // Verify login credentials
                    let validLogin = false;
                    let userRole = '';
                    
                    // Simple validation
                    if (email === 'john@example.com' && input === 'pass123') {
                        validLogin = true;
                        userRole = 'student';
                    } else if (email === 'wilson@example.com' && input === 'pass987') {
                        validLogin = true;
                        userRole = 'faculty';
                    } else if (email === 'admin@library.com' && input === 'admin123') {
                        validLogin = true;
                        userRole = 'librarian';
                    }
                    
                    if (validLogin) {
                        term.writeln(`Login successful! Welcome back, ${userRole}.`);
                        term.writeln('Loading user menu...\n');
                        
                        setTimeout(() => {
                            displayUserMenu(userRole);
                            inputState = 'user_menu';
                        }, 1000);
                    } else {
                        term.writeln('Invalid credentials! Please try again.\n');
                        
                        setTimeout(() => {
                            initializeTerminalUI();
                            inputState = 'main';
                        }, 1000);
                    }
                    break;
                    
                case 'user_menu':
                    if (input === '8') {
                        term.writeln('Logging out...\n');
                        
                        setTimeout(() => {
                            initializeTerminalUI();
                            inputState = 'main';
                        }, 1000);
                    } else {
                        term.writeln(`Selected option: ${input}`);
                        term.writeln('This is a simulated interface. In a real deployment, this would interact with the C++ backend.\n');
                        term.writeln('Press Enter to continue...');
                        inputState = 'continue';
                    }
                    break;
                    
                case 'continue':
                    if (currentUserRole) {
                        displayUserMenu(currentUserRole);
                        inputState = 'user_menu';
                    } else {
                        initializeTerminalUI();
                        inputState = 'main';
                    }
                    break;
            }
        }
        
        let currentUserRole = '';
        
        function displayUserMenu(role) {
            currentUserRole = role;
            term.clear();
            term.writeln('\x1b[1;35m╔════════════════════════╗\x1b[0m');
            term.writeln('\x1b[1;35m║        User Menu       ║\x1b[0m');
            term.writeln('\x1b[1;35m╚════════════════════════╝\x1b[0m\n');
            
            if (role === 'student' || role === 'faculty') {
                term.writeln('\x1b[1;33m  ╔─────────────╗\x1b[0m');
                term.writeln('\x1b[1;33m  ║ 1. View Books ║\x1b[0m');
                term.writeln('\x1b[1;33m  ╚─────────────╝\x1b[0m');
                term.writeln('\x1b[2m  ╔─────────────╝\x1b[0m\n');
                
                term.writeln('\x1b[1;35m  ╔──────────────╗\x1b[0m');
                term.writeln('\x1b[1;35m  ║ 2. Search Books ║\x1b[0m');
                term.writeln('\x1b[1;35m  ╚──────────────╝\x1b[0m');
                term.writeln('\x1b[2m  ╔──────────────╝\x1b[0m\n');
                
                term.writeln('\x1b[1;33m  ╔──────────────╗\x1b[0m');
                term.writeln('\x1b[1;33m  ║ 3. Borrow Book ║\x1b[0m');
                term.writeln('\x1b[1;33m  ╚──────────────╝\x1b[0m');
                term.writeln('\x1b[2m  ╔──────────────╝\x1b[0m\n');
                
                term.writeln('\x1b[1;35m  ╔─────────────╗\x1b[0m');
                term.writeln('\x1b[1;35m  ║ 8. Logout    ║\x1b[0m');
                term.writeln('\x1b[1;35m  ╚─────────────╝\x1b[0m');
                term.writeln('\x1b[2m  ╔─────────────╝\x1b[0m\n');
            } else {
                term.writeln('\x1b[1;33m  ╔────────────╗\x1b[0m');
                term.writeln('\x1b[1;33m  ║ 1. Add Book ║\x1b[0m');
                term.writeln('\x1b[1;33m  ╚────────────╝\x1b[0m');
                term.writeln('\x1b[2m  ╔────────────╝\x1b[0m\n');
                
                term.writeln('\x1b[1;35m  ╔───────────────╗\x1b[0m');
                term.writeln('\x1b[1;35m  ║ 2. Remove Book ║\x1b[0m');
                term.writeln('\x1b[1;35m  ╚───────────────╝\x1b[0m');
                term.writeln('\x1b[2m  ╔───────────────╝\x1b[0m\n');
                
                term.writeln('\x1b[1;33m  ╔────────────╗\x1b[0m');
                term.writeln('\x1b[1;33m  ║ 3. Add User ║\x1b[0m');
                term.writeln('\x1b[1;33m  ╚────────────╝\x1b[0m');
                term.writeln('\x1b[2m  ╔────────────╝\x1b[0m\n');
                
                term.writeln('\x1b[1;35m  ╔─────────────╗\x1b[0m');
                term.writeln('\x1b[1;35m  ║ 8. Logout    ║\x1b[0m');
                term.writeln('\x1b[1;35m  ╚─────────────╝\x1b[0m');
                term.writeln('\x1b[2m  ╔─────────────╝\x1b[0m\n');
            }
            
            term.write('\x1b[35mChoice: \x1b[0m');
        }
    }
    
    // In a real environment, we would connect to a WebSocket
    // Since we don't have that, we'll simulate the terminal output
    function startProcess() {
        if (socket && connected) {
            socket.send(JSON.stringify({
                type: 'command',
                command: './library_system'
            }));
        } else {
            // If no WebSocket connection, simulate the terminal
            simulateOutput();
        }
    }
    
    // Button event handlers
    clearBtn.addEventListener('click', () => {
        term.clear();
    });
    
    restartBtn.addEventListener('click', () => {
        if (socket && connected) {
            if (processRunning) {
                socket.send(JSON.stringify({
                    type: 'interrupt'
                }));
            }
            setTimeout(() => {
                startProcess();
            }, 500);
        } else {
            term.clear();
            simulateOutput();
        }
    });
    
    // Handle window resize
    window.addEventListener('resize', () => {
        fitAddon.fit();
    });
    
    // Initial setup
    try {
        connectWebSocket();
    } catch (error) {
        console.error('Failed to connect WebSocket, falling back to simulation', error);
        simulateOutput();
    }
    
    // If WebSocket fails to connect within 3 seconds, fall back to simulation
    setTimeout(() => {
        if (!connected) {
            simulateOutput();
        }
    }, 3000);
});
