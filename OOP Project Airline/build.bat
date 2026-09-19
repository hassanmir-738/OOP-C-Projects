@echo off
echo =========================================================================
echo   Compiling SkyLink Airways Airline Management System (C++)
echo =========================================================================

if not exist "data" mkdir data

g++ -std=c++14 -Wall -Wextra -Iinclude src/Flight.cpp src/DomesticFlight.cpp src/InternationalFlight.cpp src/CharterFlight.cpp src/Passenger.cpp src/EconomyPassenger.cpp src/BusinessPassenger.cpp src/FirstClassPassenger.cpp src/Ticket.cpp src/Airline.cpp src/Utils.cpp src/main.cpp -o airline_system.exe

if %ERRORLEVEL% equ 0 (
    echo [BUILD SUCCESS] Successfully compiled airline_system.exe
    echo.
    echo Launching Airline Management System...
    echo =========================================================================
    airline_system.exe
) else (
    echo.
    echo [BUILD ERROR] Compilation failed. Please ensure MinGW g++ is installed and on your PATH.
    pause
)
