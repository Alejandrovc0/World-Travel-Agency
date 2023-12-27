# World Travel Agency

CS First semester - Final C++ project. 
World Travel Agency reservation system! This program allows users to create, manage, and access their travel reservations to various destinations.

## Table of Contents

- [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Test Files](#additional-Test-Files)
- [Contributing](#contributing)
- [License](#license)

## Features

### 1. **Create a New Reservation**

   - **Destination Options:**
     - **Rio de Janeiro, Brazil:** Experience the vibrant culture and join the festivities of the Carnival.
     - **Tokyo, Japan:** Embark on an adventure to Tokyo and climb Mount Fuji with the guidance of experienced instructors.
     - **Vienna, Austria:** Explore the historical city of Vienna and take a guided tour to discover its beauty.

   - **Detailed Breakdown:** Get a comprehensive breakdown of charges, including base costs, additional services, and applicable discounts.

   - **Random Confirmation Code:** Each reservation is assigned a unique confirmation code for easy access.

### 2. **Access Reservations**

   - **Retrieve Reservation Details:** Use your name and confirmation code to access and view the details of your existing reservations.

   - **User-Friendly Interface:** The system guides you through the steps to retrieve your reservation information.

### 3. **Finish Menu Options**

   - **Create Another Reservation:** Continue creating new reservations after completing the current one.

   - **Access Reservations:** View and manage your reservations.

   - **Log Out:** Safely exit the program.

## Getting Started

### Prerequisites

- C++ compiler (e.g., g++)
- Make sure you have the necessary permissions to compile and run C++ programs.

### Installation

1. **Clone the repository:**

   ```bash
   git clone https://github.com/your-username/world-travel-agency.git
   ```

2. **Compile the program:**

   ```bash
   g++ travel_agency.cpp -o travel_agency
   ```

3. **Run the executable:**

   ```bash
   ./travel_agency
   ```

## Usage

1. **Main Menu:**
   - Select an option to log in, create a new account, or recover an existing account.

2. **Account Management:**
   - Follow prompts to complete account-related tasks.

3. **Destination Menu:**
   - Choose a destination and provide details for the reservation.

4. **Reservation Breakdown:**
   - Review the detailed breakdown of charges and save the reservation.

5. **Access Reservations:**
   - Use the "Access reservations" option to view the details of an existing reservation.

6. **Finish Menu:**
   - Select options to create another reservation, access existing reservations, or log out.

## Project Structure

- `main.cpp`: Main program file containing the entry point and core logic.
- `account.h` and `account.cpp`: Header and implementation files for user account management.
- `reservation.h` and `reservation.cpp`: Header and implementation files for reservation management.
- `travel.h` and `travel.cpp`: Header and implementation files for travel-related functions.

## Additional Test Files
The repository includes test files to validate the functionality of the reservation system - see the [Tests](Tests) file:
- `test.txt`: Contains a sample reservation for a user.
- `test2.txt`: Another sample reservation with different options.

To test the system with these files, follow these steps:
1. Run the program.
2. Select the option to access reservations.
3. Enter the name on the reservation and the confirmation code.
4. The system will display the details of the matching reservation.

## Authors

- Juan Valencia

## Contributing

Contributions are welcome! Please fork the repository and create a pull request with your improvements. For major changes, please open an issue first to discuss the proposed changes.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
