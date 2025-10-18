#ifndef AUTH_H
#define AUTH_H

enum class AuthResult { Ok, UserNotFound, WrongPassword, AlreadyExists };
enum class CheckAuth {OK, NOT};

enum class VerifyAuth {OK, NOT}; // client

enum class AuthState { AUTHORIZATION, TRANSACTION, UPDATE };
struct  AuthSession {
  AuthState state = AuthState::AUTHORIZATION;
  std::string pendingUser;  // lưu USER chờ PASS
  bool authed = false;
};


#endif // AUTH_H