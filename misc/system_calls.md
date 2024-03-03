# Runtime System Calls

@tableofcontents

## Under The Hood

Rishka system calls provide a mechanism for user-level applications to request services or functionality from the underlying operating system (Jessy OS) running on the ESP32-WROVER microcontroller. Through these system calls, applications can request various services, functionalities, or access to system resources that are otherwise restricted from user-level code. This includes operations such as file I/O, memory management, device control, and system information retrieval. By leveraging system calls, applications can harness the full capabilities of the operating system while maintaining a clear separation between user-level and kernel-level code, ensuring system integrity, security, and reliability.

1. **Invocation**

    User-level applications on the ESP32-WROVER microcontroller can request privileged operations or access system-level functionality through system calls. These requests are made by the application directly to the underlying operating system, Jessy OS. Rather than executing system calls directly, applications rely on an abstraction layer provided by the Rishka SDK to facilitate communication with the operating system.

2. **Arguments**

    When invoking a system call, the user-level application provides any necessary parameters or data to the operating system. These parameters convey important information to the system call handler, enabling it to perform the requested operation. The specific arguments required for each system call depend on the nature of the operation being requested and are documented in the Rishka SDK.

3. **Registers**

    Before initiating a system call, the user-level application prepares the arguments and system call identifier according to the RISC-V calling convention. This convention dictates the usage of specific registers to pass function arguments and system call identifiers to the operating system. By adhering to this convention, applications ensure that the operating system can correctly interpret and process the provided information.

4. **Triggering the System Call**

    Once the necessary arguments are prepared, the user-level application initiates the system call by triggering a special instruction known as the scall. This instruction serves as a signal to the processor that a system call is being requested. Upon encountering the scall instruction, the processor transitions from user mode to privileged mode and transfers control to the operating system's system call handler.

5. **System Call Handler**

    The system call handler within the operating system receives control when the scall instruction is encountered. It is responsible for interpreting the system call identifier provided by the user-level application and executing the corresponding functionality or service. The handler interacts with kernel resources, hardware peripherals, and other system components as necessary to fulfill the requested operation.

6. **Result**

    After processing the system call, the system call handler returns the result of the operation to the user-level application. This result communicates the outcome of the system call execution, indicating whether the requested operation was successful or encountered an error. The user-level application can then proceed with its execution based on this feedback.

7. **Return to User-Level Code**

    Upon completing the system call handler's execution, control returns to the user-level application. The application resumes its execution, continuing with its tasks or responding to the outcome of the system call as needed. This seamless return to user-level code ensures that applications can interact with the operating system efficiently and access privileged functionality while maintaining a clear separation between user and kernel space.

## Complete List of System Calls

The Rishka virtual runtime kernel provides an extensive array of system calls, offering a rich set of functionalities and services for user-level applications on the ESP32-WROVER microcontroller platform.

The below table is a comprehensive listing with details of the diverse range of system calls available within the Rishka kernel. Note that this is subject to changes since Rishka is still underdevelopment.

| Address (A7) | Name        | A0              | A1        | A2        | A3        | Implementation       |
|--------------|-------------|-----------------|-----------|-----------|-----------|----------------------|
| 0x00         | PRINTS      | (string) text   |           |           |           | IO::print(string)    |
| 0x01         | PRINTN      | (i64) number    |           |           |           | IO::print(i64)       |
| 0x02         | PRINTD      | (double) number |           |           |           | IO::print(double)    |
| 0x03         | READCH      |                 |           |           |           | IO::readch()         |