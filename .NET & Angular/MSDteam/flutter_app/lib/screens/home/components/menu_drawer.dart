import 'package:flutter/material.dart';
import 'package:flutter_app/constants.dart';

class MenuDrawer extends StatelessWidget {
  const MenuDrawer({
    Key? key,
  }) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Drawer(
      backgroundColor: kBackgroundColor,
      // ListView allows scrolling inside the drawer
      child: ListView(
        padding: EdgeInsets.zero,
        children: <Widget>[
          const SizedBox(
            height: 50,
          ),
          ListTile(
            title: const Center(
                child: Text(
              'ACCOUNT',
              style: kMenuItemTextStyle,
            )),
            onTap: () {
              // Update the state of the app
              

              // Close the drawer
              Navigator.pop(context);
            },
          ),
          ListTile(
            title: const Center(
                child: Text(
              'WATCHLIST',
              style: kMenuItemTextStyle,
            )),
            onTap: () {
              Navigator.pop(context);
            },
          ),
          ListTile(
            title: const Center(
                child: Text(
              'LIKES',
              style: kMenuItemTextStyle,
            )),
            onTap: () {
              Navigator.pop(context);
            },
          ),
          ListTile(
            title: const Center(
                child: Text(
              'DISLIKES',
              style: kMenuItemTextStyle,
            )),
            onTap: () {
              Navigator.pop(context);
            },
          ),
        ],
      ),
    );
  }
}
