# air-compressor-controller
Arduino-Controlled 3 Compressor System 

This project uses an Arduino to monitor pressure sensors and independently control three air compressors through relays. It ensures that each compressor only turns on when needed, while also managing the overall system load safely through a main contactor.

Parts List
•	1 × Arduino Uno (or similar microcontroller)
•	3 × Solid State Relays (SSR) rated for 25A or higher
•	1 × Main Contactor rated for 70A or higher (combined load)
•	3 × Pressure Sensors (0-150 PSI typical range)
•	1 × 5V Power Supply for Arduino
•	Appropriate gauge wiring (recommend 10 AWG for compressors)
•	DIN Rail and enclosures for mounting (optional but highly recommended)

Schematic Overview
• Each compressor is connected to its own SSR.
• Each SSR is triggered by an Arduino output pin.
• Pressure sensors send analog signals to Arduino input pins.
• The Arduino reads pressure values and decides when to turn each compressor on or off.
• All compressor power feeds through a main contactor for added safety.
• The main contactor is activated together with compressor activation.

Basic Wiring Description
• Each pressure sensor is wired to an analog input on the Arduino (e.g., A0, A1, A2).
• Arduino digital outputs (e.g., D2, D3, D4) connect to SSR control inputs.
• SSRs switch the high voltage (110V/220V) AC power to the compressors.
• Main contactor provides overall power enable/disable.
• Arduino logic continuously monitors pressures and switches relays.




Arduino Pseudocode (Detailed)
Setup:
• Define compressor control pins (D2, D3, D4) as OUTPUT.
• Define pressure sensor pins (A0, A1, A2) as INPUT.
• Initialize compressor state variables (compressor1State, compressor2State, compressor3State) to false.
• Initialize compressor waiting flags (compressor1Waiting, compressor2Waiting, compressor3Waiting) to false.

Loop:
• Read pressure sensors A0, A1, A2.
• Count how many compressors are currently ON (compressor1State, compressor2State, compressor3State).
• For each compressor:
    - If pressure is below the low threshold:
        - If fewer than 2 compressors are running:
            - Turn compressor ON.
            - Set compressor state to true.
        - Else:
            - Set compressor waiting flag to true.
    - If pressure is above the high threshold:
        - Turn compressor OFF.
        - Set compressor state and waiting flag to false.

• After any compressor turns OFF:
    - If any compressor is waiting:
        - Turn ON the waiting compressor immediately.
        - Update its state and clear its waiting flag.

• Repeat continuously with a short delay.
