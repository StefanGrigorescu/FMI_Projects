import 'package:flutter/material.dart';
import 'package:flutter_app/constants.dart';
import 'package:flutter_app/screens/home/components/body.dart';
import 'package:flutter_app/screens/home/components/menu_drawer.dart';

import 'components/bottom_actions.dart';

class Homescreen extends StatefulWidget {
  const Homescreen({Key? key}) : super(key: key);

  @override
  _HomescreenState createState() => _HomescreenState();
}

class _HomescreenState extends State<Homescreen> {
  int currentPage = 0;
  PageController pageController = PageController(initialPage: 0);

  final _scaffoldKey = new GlobalKey<ScaffoldState>();

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      drawerEnableOpenDragGesture: false,
      drawerEdgeDragWidth: 0,
      key: _scaffoldKey,
      appBar: buildAppBar(),
      endDrawer: const MenuDrawer(),
      body: PageView(
        controller: pageController,
        onPageChanged: (int val) => setPage(val),
        children: const <Body>[
          Body(movie: {
            'title': 'Bumblebee',
            'year': '2018',
            'score': '6.7',
            'number_of_reviews': '156k',
            'description':
                "On the run in the year 1987, Bumblebee finds refuge in a junkyard in a small California beach town. On the cusp of turning 18 and trying to find her place in the world, Charlie Watson discovers Bumblebee, battle-scarred and broken.",
            'poster':
                "https://m.media-amazon.com/images/M/MV5BMjUwNjU5NDMyNF5BMl5BanBnXkFtZTgwNzgxNjM2NzM@._V1_.jpg",
          }),
          Body(movie: {
            'title': 'Cars 3',
            'year': '2017',
            'score': '6.7',
            'number_of_reviews': '91k',
            'description':
                "Lightning McQueen sets out to prove to a new generation of racers that he's still the best race car in the world.",
            'poster':
                "https://cdn.europosters.eu/image/750/cars-3-duel-i97645.jpg",
          }),
          Body(movie: {
            'title': 'Megamind',
            'year': '2010',
            'score': '7.3',
            'number_of_reviews': '249k',
            'description':
                "Evil genius Megamind finally defeats his do-gooder nemesis, Metro Man, but is left without a purpose in a superhero-free world.",
            'poster':
                "https://imgc.allpostersimages.com/img/posters/megamind_u-L-F3WOKT0.jpg?artHeight=550&artPerspective=n&artWidth=550"
          }),
        ],
      ),
      bottomNavigationBar: BottomActions(
        onPageChanged: (int val) => changePage(val),
      ),
    );
  }

  AppBar buildAppBar() {
    return AppBar(
      backgroundColor: kAppBarColor,
      shadowColor: Colors.black,
      iconTheme: const IconThemeData(
        color: kPrimaryColor,
        size: 40,
      ),
    );
  }

  // Changes the current page by val
  // -1 if back or 1 if forward
  changePage(int val) {
    // print("Change page with $val");
    setState(() {
      // Check for negative page count
      if (currentPage + val < 0) return;

      // TODO: Check for greater page then existing ones and generate one if necesary

      currentPage += val;
    });

    scrollToCurrentPage(); // Animate scrolling to new current page
  }

  // Scroll animation to the current page
  scrollToCurrentPage() {
    pageController.animateToPage(currentPage,
        duration: const Duration(milliseconds: 200), curve: Curves.bounceInOut);
  }

  // Sets the page to a specified value
  // It is called by swiping so no animation is required
  setPage(int val) {
    // print("Set page to $val");
    setState(() {
      currentPage = val;
    });
  }
}
