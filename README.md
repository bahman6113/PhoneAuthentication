# Windows Authentication Using Mobile App

Phone authenticator is a project developed for two-factor authentication under Windows OS(Windows 7 and higher versions are supported). The idea
behind the project is like this. The user wants to login into his/her system. The system will show a randomly generate string(nonce) to the user.
User should enter the generated nonce into the mobile app and the app will generate a one time password for the user.


The project consists of three parts. The first part is an authentication provider which will be installed on the user PC's windows. The authentication
provider has been developed using VC++. The second part is the mobile app which is developed using Xamarin for android. Visual studio is the IDE used for
developing the mobile app. The third component is a Configurator which stores user's credentials(user + domain + password) in the system. User credentials 
will be encrypted before storing. The component is developed using VC++. 

Due to time limitations and need of having a very first version of the project, the first version  uses a hard-coded symmetric key for generating one time passwords and encrypting user's credentials and it only supports single user.  In the next versions, RSA keys and digital certificates will be
used for generating the code and encryption of users' credential.

## Getting Started

There are 3 VS solutions in the project. Clone or fork the projects and build them using Visual Studio 2015.

### Prerequisites

The authentication provider and Configurator use OpenSSL for cryptographic operation. To build the projects you need to set OPENSSL_PATH environment variable
pointing to the root folder of OpenSSL. The root folder should contain a lib folder which contains libeay32.lib file. You also need to install Visual C++ Redistributable packages. For running the authenticator and configurator under Windows OS with X64 architecture, you need to compile them for X64 architecture.  To compile the mobile app, you will need to install Xamarin.  


### Installing

Compiling the authentication provider will generate a SampleV2CredentialProvider.dll file. Copy the file into System32 folder and run register.reg which is available in authentication provider's folder. In the same way, run unregister.reg if you need to uninstall the provider. Output of CP Configurator is PhoneAuth Configurator.exe . Run the configurator under administrator privileges and enter user name and password of the user whom you want to login using one-time password.


## Built With

* Visual Studio 2015 Community (https://www.visualstudio.com/vs/)
* OpenSSL 1.0.2K (https://www.openssl.org/source/)
* Xamarin for windows (https://developer.xamarin.com/guides/cross-platform/windows/)

## Authors

**Bahman Javadi** - (https://github.com/bahman6113/PhoneAuthentication)
bahman.javadi.es@gmail.com

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details
