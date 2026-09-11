import 'package:flutter_app/models/auth_model.dart';
import 'package:flutter_secure_storage/flutter_secure_storage.dart';

class SecureStorage {
  final _storage = const FlutterSecureStorage();

  Future writeSecureData(String key, String value) async {
    var writeData = await _storage.write(key: key, value: value);
    return writeData;
  }

  Future readSecureData(String key) async {
    var readData = await _storage.read(key: key);
    return readData;
  }

  Future deleteSecureData(String key) async {
    var deleteData = await _storage.delete(key: key);
    return deleteData;
  }

  Future writeAuthModel(AuthModel authModel) async {
    await writeSecureData("accessToken", authModel.accessToken);
    await writeSecureData("refreshToken", authModel.refreshToken);
  }

  Future<AuthModel> readAuthModel() async {
    var accessToken = await readSecureData("accessToken");
    var refreshToken = await readSecureData("refreshToken");
    return AuthModel(accessToken: accessToken, refreshToken: refreshToken);
  }
}
