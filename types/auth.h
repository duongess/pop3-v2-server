#ifndef AUTH_H
#define AUTH_H

enum class AuthResult { Ok, UserNotFound, WrongPassword, AlreadyExists };
enum class CheckAuth {OK, NOT};

enum class VerifyAuth {OK, NOT}; // client

#endif // AUTH_H