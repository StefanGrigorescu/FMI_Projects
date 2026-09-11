import 'package:flutter/material.dart';

// Color scheme for app
const kPrimaryColor = Color.fromRGBO(246, 199, 0, 1);
const kBackgroundColor = Color.fromRGBO(23, 21, 21, 1);
const kTextColor = Color.fromARGB(255, 255, 255, 255);
const kIconColor = Color.fromARGB(255, 0, 0, 0);

const kAppBarColor = kBackgroundColor;

const kImageBorderColor = Color.fromARGB(255, 255, 255, 255);
const kImageBoarderRadius = 20.0;

const double kDefaultPadding = 20.0;

const kMovieTitleStyle = TextStyle(
  fontFamily: "Work Sans",
  fontSize: 24.0,
);

const kIMDBRatingStyle = TextStyle(
  fontFamily: "Work Sans",
  fontSize: 14.0,
  letterSpacing: 2.0,
);

const kMovieDescriptionStyle = TextStyle(
  fontFamily: "Roboto",
  fontSize: 14.0,
);

const kLinkStyle = TextStyle(
  fontFamily: "Roboto",
  fontSize: 14.0,
  color: Colors.grey,
);

const kAuthStyle = TextStyle(
  fontFamily: "Roboto",
  fontSize: 16.0,
  color: kTextColor,
);

const kButtonTextStyle = TextStyle(
  fontFamily: "Roboto",
  fontSize: 16.0,
  color: kBackgroundColor,
);

const kDialogTitle = TextStyle(
  fontFamily: "Roboto",
  fontSize: 20.0,
  color: Colors.black,
);

const kDialogDescription = TextStyle(
  fontFamily: "Roboto",
  fontSize: 16.0,
  color: Colors.black,
);

const kMenuItemTextStyle = TextStyle(
  fontFamily: "Roboto",
  fontSize: 18.0,
  color: kPrimaryColor,
);

const kFormFieldColor = kPrimaryColor;
const kFormFieldIconColor = Color.fromARGB(255, 255, 255, 255);

// Debug mode
const kDebugMode = true;

// API
const kAuthRequestURL = "https://localhost:5003/api/Authentication/";
