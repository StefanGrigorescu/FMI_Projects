import 'dart:convert';
import 'dart:developer';

import 'package:flutter/material.dart';
import 'package:flutter_app/constants.dart';
import 'package:flutter_app/models/auth_model.dart';
import 'package:flutter_app/services/auth_service.dart';
import 'package:flutter_app/services/secure_storage.dart';

class LoginForm extends StatefulWidget {
  const LoginForm({Key? key}) : super(key: key);

  @override
  _LoginFormState createState() => _LoginFormState();
}

class _LoginFormState extends State<LoginForm> {
  // global key for this form
  final _formKey = GlobalKey<FormState>();

  // if the password is visible or not
  bool _isPasswordVisible = false;

  // controllers for form fields
  final TextEditingController _emailController = TextEditingController();
  final TextEditingController _passwordController = TextEditingController();

  // service for calling the API
  final AuthService _authService = AuthService();

  // service for secure storage
  final SecureStorage _secureStorage = SecureStorage();

  @override
  Widget build(BuildContext context) {
    Size size = MediaQuery.of(context).size; // Screen size

    return Form(
      key: _formKey,
      child: Center(
        child: SizedBox(
          width: size.width * 0.8,
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: <Widget>[
              // Email or username field
              TextFormField(
                controller: _emailController,
                autocorrect: false,
                decoration: const InputDecoration(
                  enabledBorder: OutlineInputBorder(
                    borderSide: BorderSide(
                      color: kFormFieldColor,
                    ),
                  ),
                  focusedBorder: OutlineInputBorder(
                    borderSide: BorderSide(
                      color: kFormFieldColor,
                    ),
                  ),
                  border: OutlineInputBorder(),
                  labelStyle: kAuthStyle,
                  hintStyle: kAuthStyle,
                  labelText: 'Email or username',
                  hintText: 'Enter your email or username',
                ),
                validator: (value) {
                  if (value == null || value.isEmpty) {
                    return '* requiered';
                  }
                  return null;
                },
              ),

              // Password field
              Padding(
                padding: const EdgeInsets.symmetric(vertical: kDefaultPadding),
                child: TextFormField(
                  controller: _passwordController,
                  autocorrect: false,
                  // Form field aspect
                  decoration: InputDecoration(
                    enabledBorder: const OutlineInputBorder(
                      borderSide: BorderSide(
                        color: kFormFieldColor,
                      ),
                    ),
                    focusedBorder: const OutlineInputBorder(
                      borderSide: BorderSide(
                        color: kFormFieldColor,
                      ),
                    ),
                    border: const OutlineInputBorder(),
                    labelStyle: kAuthStyle,
                    hintStyle: kAuthStyle,
                    labelText: 'Password',
                    hintText: 'Enter your password',
                    suffixIcon: IconButton(
                        // Toggle if password is visible or not
                        onPressed: () {
                          setState(() {
                            _isPasswordVisible = !_isPasswordVisible;
                          });
                        },
                        // Icon based on the state of _isPasswordVisible
                        icon: Icon(
                          _isPasswordVisible
                              ? Icons.visibility_off
                              : Icons.visibility,
                          color: kFormFieldIconColor,
                        )),
                  ),
                  obscureText: !_isPasswordVisible,
                  validator: (value) {
                    if (value == null || value.isEmpty) {
                      return '* requiered';
                    }
                    return null;
                  },
                ),
              ),

              // Submit button
              ElevatedButton(
                style: ElevatedButton.styleFrom(
                  primary: kPrimaryColor,
                  enableFeedback: true,
                ),
                onPressed: () async {
                  // Check if the form is valid
                  if (_formKey.currentState!.validate()) {
                    // Call API
                    var res = await _authService.login(
                        _emailController.text, _passwordController.text);

                    if (kDebugMode) {
                      print(res.statusCode);
                    }
                    if (res.statusCode == 200) {
                      // TODO: Show a progress indicator

                      var response = jsonDecode(res.body);
                      var authModel = AuthModel.fromJson(response);

                      // Update in secure storage
                      if (kDebugMode) {
                        print(inspect(authModel));
                      }
                      _secureStorage.writeAuthModel(authModel);

                      // Go to homepage
                      Navigator.pushNamed(context, "/home");
                    } else {
                      displayDialog("Login error",
                          "Please make sure your email and password are correct.");
                    }
                  }
                },
                child: const Text(
                  'LOGIN',
                  style: kButtonTextStyle,
                ),
              ),

              TextButton(
                style: TextButton.styleFrom(
                  textStyle: kLinkStyle,
                ),
                child: const Text(
                  "Don't have an account? Sign up here",
                  style: kLinkStyle,
                ),
                onPressed: () {
                  // Go to register page
                  Navigator.pushNamed(context, "/register");
                },
              )
            ],
          ),
        ),
      ),
    );
  }

  @override
  void dispose() {
    // Clean up the controller when the widget is disposed.
    _emailController.dispose();
    _passwordController.dispose();
    super.dispose();
  }

  void displayDialog(String title, String description) => showDialog(
        context: context,
        builder: (context) => AlertDialog(
          actions: [
            TextButton(
              onPressed: () {
                Navigator.pop(context); // closes dialog
              },
              child: const Text(
                "OK",
              ),
            )
          ],
          title: Text(
            title,
            style: kDialogTitle,
          ),
          content: Text(
            description,
            style: kDialogDescription,
          ),
        ),
      );
}
