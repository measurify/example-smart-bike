import 'dart:math';

import 'package:flutter/material.dart';
import 'package:flutter_http/altitude_speedData.dart';
import 'package:flutter_http/http_service.dart';

class FeaturesPage extends StatelessWidget {
  final HttpService httpService = HttpService();

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("Smart Bike Data"),
        centerTitle: true,
        backgroundColor: Colors.black,
      ),
      body: FutureBuilder(
          future: httpService.getFeature(httpService.getToken()),
          builder: (context, snapshot) {
            if (snapshot.hasData) {
              return showBikeData(snapshot.data);
            }
            return Center(child: CircularProgressIndicator());
          }),
    );
  }

  Widget showBikeData(Map feature) {
    //calculate the maximum value in speed list values
    List<SpeedData> speedListData = feature['Speed'];
    List<double> speedList = [];
    for (int i = 0; i < speedListData.length; i++) {
      speedList.add(speedListData[i].speed);
    }
    double maxSpeed = speedList.reduce(max);

    //dispaly all the feature with icon and value,
    //with charts for altitude and speed
    return SizedBox(
      height: 1800,
      child: Card(
        child: ListView(
          children: [
            Image.asset(
              'Images/strive.png',
              width: 350,
              height: 300,
            ),
            ListTile(
              title: Text(
                  'Number of fork compression: ' +
                      feature['Fork compressions'][0].round().toString(),
                  style: TextStyle(fontWeight: FontWeight.w700, fontSize: 18)),
              leading: Icon(Icons.pedal_bike, color: Colors.red[900], size: 30),
            ),
            Divider(),
            ListTile(
              title: Text(
                  'Max lean angle: ' +
                      feature['Lean angle'][0].toString() +
                      '°',
                  style: TextStyle(fontSize: 18, fontWeight: FontWeight.w700)),
              leading: Icon(Icons.rotate_90_degrees_ccw_outlined,
                  color: Colors.red[900], size: 30),
            ),
            Divider(),
            ListTile(
              title: Text(
                  'Max slope angle: ' +
                      feature['Slope angle'][0].toString() +
                      '°',
                  style: TextStyle(fontWeight: FontWeight.w700, fontSize: 18)),
              leading: Icon(Icons.text_rotation_angledown,
                  color: Colors.red[900], size: 30),
            ),
            Divider(),
            ListTile(
              title: Text(
                  'Time: ' +
                      (feature['Time'][0] / 60).floor().toString() +
                      ' min ' +
                      (feature['Time'][0] % 60).toString() +
                      ' sec',
                  style: TextStyle(fontWeight: FontWeight.w700, fontSize: 18)),
              leading: Icon(Icons.timer, color: Colors.red[900], size: 30),
            ),
            Divider(),
            ListTile(
              title: Text('Max speed: ' + maxSpeed.toString() + ' km/s',
                  style: TextStyle(fontWeight: FontWeight.w700, fontSize: 18)),
              leading:
                  Icon(Icons.speed_outlined, color: Colors.red[900], size: 30),
            ),
            Divider(),
            ListTile(
              title: Text(
                  'Space traveled: ' +
                      feature['Space travelled'][0].toString() +
                      ' km',
                  style: TextStyle(fontWeight: FontWeight.w700, fontSize: 18)),
              leading:
                  Icon(Icons.edit_road_sharp, color: Colors.red[900], size: 30),
            ),
            AltitudeDataChart(feature['Altitude']),
            SpeedDataChart(feature['Speed']),
          ],
        ),
      ),
    );
  }
}
