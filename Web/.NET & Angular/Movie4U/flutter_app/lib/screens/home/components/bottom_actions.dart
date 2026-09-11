import 'package:flutter/material.dart';
import 'package:flutter_app/constants.dart';

class BottomActions extends StatelessWidget {

  final Function(int) onPageChanged;

  const BottomActions({Key? key, required this.onPageChanged}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Container(
      color: kPrimaryColor,
      height: 80.0,
      child: Row(
        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
        children: <Widget>[
          IconButton(
            onPressed: (){
              onPageChanged(-1);
            },
            iconSize: 40,
            icon: const Icon(
              Icons.arrow_back_ios_outlined,
              color: kIconColor,
            ),
          ),
          IconButton(
            onPressed: dislikeMovie(),
            iconSize: 40,
            icon: const Icon(
              Icons.sentiment_dissatisfied_outlined,
              color: kIconColor,
            ),
          ),
          IconButton(
            onPressed: addToWatchlist(),
            iconSize: 50,
            icon: const Icon(
              Icons.add_circle,
              color: kIconColor,
            ),
          ),
          IconButton(
            onPressed: likeMovie(),
            iconSize: 40,
            icon: const Icon(
              Icons.sentiment_satisfied_outlined,
              color: kIconColor,
            ),
          ),
          IconButton(
            onPressed: (){
              onPageChanged(1);
            },
            iconSize: 40,
            icon: const Icon(
              Icons.arrow_forward_ios_outlined,
              color: kIconColor,
            ),
          ),
        ],
      ),
    );
  }
    
  dislikeMovie() {
    // TODO
  }

  likeMovie() {
    // TODO
  }

  addToWatchlist() {
    // TODO
  }

}

