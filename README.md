# ATM Management System

A robust and secure ATM management system implemented in C, featuring comprehensive account management, transaction handling, and user authentication.

## Features

- User authentication (login/registration)
- Account management
  - Create new accounts
  - Update account information
  - Check account details
  - Remove accounts
  - Transfer ownership
- Transaction handling
  - Deposits
  - Withdrawals
- Multiple account types
  - Savings accounts
  - Fixed-term accounts (1, 2, and 3 years)
- Security features
  - Password protection
  - Failed login attempt handling
  - Data validation
  - File backup system

## Building and Running

### Prerequisites

- C compiler (gcc recommended)
- Make
- Doxygen (for documentation generation)

### Building

```bash
make
```

### Running

```bash
./atm
```

### Generating Documentation

```bash
doxygen Doxyfile
```

The generated documentation will be available in the `docs` directory.

## Security Features

- Input validation for all user inputs
- Secure password handling
- File backup before modifications
- Protection against unauthorized access
- Prevention of invalid operations on fixed-term accounts
- Confirmation required for account deletion

## Error Handling

The system implements comprehensive error handling for:
- Invalid user inputs
- File operations
- Account operations
- Transaction operations
- Authentication failures

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

