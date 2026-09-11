class AuthModel {
  final String accessToken;
  final String refreshToken;

  const AuthModel({
    required this.accessToken,
    required this.refreshToken,
  });

  factory AuthModel.fromJson(Map<String, dynamic> json) {
    return AuthModel(
        accessToken: json['accessToken'], refreshToken: json['refreshToken']);
  }

  @override
  String toString() {
    return {
      "accessToken": accessToken,
      "refreshToken": refreshToken,
    }.toString();
  }
}
