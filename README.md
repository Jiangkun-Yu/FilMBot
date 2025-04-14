# 📦 FilMBot – A Film-based Electromagnetic Micromanipulator
FilMBot (Film-based ElectroMagnetic driven Robot) is an open-source, low-cost, and DIY-friendly micromanipulator. Designed for high-resolution micromanipulation tasks, FilMBot combines electromagnetic actuation with a soft, film-based kinametic structure to achieve sub-10 μm precision and fast response, all at a total cost of around €100.

This repository provides all necessary hardware files, firmware, and fabrication guidance to replicate and customize FilMBot for use in research, education, or hobby projects.

📄 You can find the original research paper describing FilMBot here: https://doi.org/10.48550/arXiv.2410.23059

## 📁 Project Structure

FilMBot/<br>
│<br>
├── hardware/               &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;# Electronics & mechanical design<br>
│   &emsp;&emsp;├── 3D models/           &ensp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;# STL files for 3D printed parts<br>
│   &emsp;&emsp;├── cut patterns/        &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;# Laser-cut/printable patterns for film structure<br>
│   &emsp;&emsp;└── schematic and PCB    &npsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;# PCB schematics, board layout, Gerber files ... <br>
│   		&emsp;&emsp;&emsp;&emsp;&emsp;├── 01 Gerber file for ordering PCB/<br>
│   		&emsp;&emsp;&emsp;&emsp;&emsp;├── 02 schematic and bill of electronic components.pdf/<br>
│   		&emsp;&emsp;&emsp;&emsp;&emsp;├── 03 Mega 2560 Rev3_datasheet_from docs.arduino.cc.pdf/<br>
│   		&emsp;&emsp;&emsp;&emsp;&emsp;├── 04 TB6612FNG Hookup Guide.txt/	<br>
│   		&emsp;&emsp;&emsp;&emsp;&emsp;└── 05 TB6612FNG datasheet.pdf/<br>
│<br>
├── firmware/                &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;# Arduino firmware<br>
│   &emsp;&emsp;└── FilMBot_Arduino_Code/ <br>
│<br>
├── materials, components and fabrication/     &emsp;&emsp;&emsp;# Cost analysis & fabrication details<br>
│   &emsp;&emsp;└── materials, components and fabrication.docx/<br>
│<br>
└── README.md                &emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;&emsp;# Project overview (this file)<br>

## 🛠️ Hardware Overview
The hardware system includes:

&emsp;&emsp;&emsp;&emsp;&emsp;Microcontroller: Arduino Mega 2560 or compatible ATmega2560 board

&emsp;&emsp;&emsp;&emsp;&emsp;Coil driver: TB6612FNG dual-channel motor drivers

&emsp;&emsp;&emsp;&emsp;&emsp;Power: 12V DC input with step-down converters (to 5V, 3.3V, etc.)

&emsp;&emsp;&emsp;&emsp;&emsp;Interfaces: Joystick + buttons, optional LCD and sensors (current, temperature)

&emsp;&emsp;&emsp;&emsp;&emsp;Structure: Soft film-based body (laser-cut or hand-cut) with permanent magnets

&emsp;&emsp;&emsp;&emsp;&emsp;PCB: Custom-designed for compact and stable operation (or breadboard prototype)

You’ll find:

&emsp;&emsp;&emsp;&emsp;&emsp;📐 3D models in hardware/3D models/

&emsp;&emsp;&emsp;&emsp;&emsp;✂️ Cutting patterns in hardware/cut patterns/

&emsp;&emsp;&emsp;&emsp;&emsp;🔌 Circuit diagrams, PCB, and related instructions and datasheets in hardware/schematic and PCB/


## 💻 Firmware
The firmware supports two control modes:

&emsp;&emsp;&emsp;&emsp;&emsp;Joystick-based telemanipulation (standalone)

&emsp;&emsp;&emsp;&emsp;&emsp;PC command-based control via UART

It runs on a max. 1 kHz (100 Hz by default) interrupt loop for:

&emsp;&emsp;&emsp;&emsp;&emsp;Reading joystick, buttons, and sensors (current, temperature)

&emsp;&emsp;&emsp;&emsp;&emsp;Performing PID control for coil current

&emsp;&emsp;&emsp;&emsp;&emsp;Generating PWM outputs

&emsp;&emsp;&emsp;&emsp;&emsp;Sending data and debug info to LCD or PC

The firmware is modular and easy to extend. To modify or maintain:

&emsp;&emsp;&emsp;&emsp;&emsp;Change pin mappings or parameters in pin_init.h

&emsp;&emsp;&emsp;&emsp;&emsp;Add new features or control logic in FilMBot_Arduino_Code.ino


## 📉 Materials, Components & Fabrication
The total cost of FilMBot is approximately €100. All parts are selected for affordability and global availability.

In materials_and_costs/, you’ll find:

&emsp;&emsp;&emsp;&emsp;&emsp;✔️ A full Bill of Materials with prices

&emsp;&emsp;&emsp;&emsp;&emsp;🔧 A DIY fabrication guide describing each step (cutting, assembly, etc.)

&emsp;&emsp;&emsp;&emsp;&emsp;🛒 A list of suggested vendors and links

FilMBot is designed to be built with only basic tools, with no need for a CNC or cleanroom.

## 🤝 License and Citation
Please cite the original paper when using this work in academic or public projects to help spread the impact of this open-source initiative:

https://doi.org/10.48550/arXiv.2410.23059

## 🌟 How to Contribute
We welcome contributions from the community! You can help by:

&emsp;&emsp;&emsp;&emsp;&emsp;Submitting pull requests to improve documentation, firmware, or hardware files

&emsp;&emsp;&emsp;&emsp;&emsp;Sharing your builds and variations

&emsp;&emsp;&emsp;&emsp;&emsp;Reporting issues or suggesting enhancements

Let’s work together to make micromanipulation more accessible and innovative!

## 📬 Contact
Feel free to reach out to the project author:

Jiangkun Yu

Doctoral Researcher, Aalto University

📧 jiangkun.yu@aalto.fi

🌐 www.linkedin.com/in/jiangkun-yu-5265a0278
