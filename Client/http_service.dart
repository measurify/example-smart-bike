import 'dart:convert';

import 'package:flutter_http/altitude_speedData.dart';

import 'feature_model.dart';
import 'package:http/http.dart';

class HttpService {
  final String url =
      'http://students.atmosphere.tools/v1/measurements?filter={\"thing\":\"mybike\"}&limit=1&page=1';
  final featureValue = new Map<String,
      List<dynamic>>(); //final featureValue = new List<String>();
  List<AltitudeData> altitude = [];
  List<SpeedData> speed = [];
  List<dynamic> time = [];
  List<dynamic> spaceTravelled = [];
  List<dynamic> slopeAngle = [];
  List<dynamic> leanAngle = [];
  List<dynamic> forkCompressions = [];

  Future<String> getToken() async {
    String data =
        "{\"username\":\"smartbike-monitor-user-username\",\"password\":\"smartbike-monitor-user-password\"}"; //body
    int length = data.length;

    Response res = await post(
      'http://students.atmosphere.tools/v1/login',
      headers: <String, String>{
        'Content-Type': 'application/json',
        'Content-Length': jsonEncode(length),
        'Host': 'students.atmosphere.tools',
        'Connection': 'keep-alive'
      },
      body: data,
    );

    if (res.statusCode == 200) {
      // If the server did return a 201 CREATED response,
      // then parse the JSON.
      var oggetto = Token.fromJson(jsonDecode(res.body));
      return oggetto.token;
    } else {
      // If the server did not return a 201 CREATED response,
      // then throw an exception.
      throw Exception('Failed to get Token');
    }
  }

  Future<Map> getFeature(Future<String> token) async {
    final String tokenString = await token;
    Response res = await get(url, headers: <String, String>{
      'Host': 'students.atmosphere.tools',
      'Connection': 'keep-alive',
      'Authorization': tokenString,
    });

    if (res.statusCode == 200) {
      var rest = json.decode(res.body)['docs'];
      var samples = rest[0]['samples'][0]['values'];

      for (int i = 0; i < samples[0].length; i++) {
        altitude
            .add(AltitudeData(altitude: samples[1][i].toDouble(), space: i));
        featureValue['Altitude'] = altitude;
        speed.add(SpeedData(speed: samples[0][i].toDouble(), space: i));
        featureValue['Speed'] = speed;
      }
      time.add(samples[2]);
      featureValue['Time'] = time;
      spaceTravelled.add(samples[3]);
      featureValue['Space travelled'] = spaceTravelled;
      slopeAngle.add(samples[4]);
      featureValue['Slope angle'] = slopeAngle;
      leanAngle.add(samples[5]);
      featureValue['Lean angle'] = leanAngle;
      forkCompressions.add(samples[6]);
      featureValue['Fork compressions'] = forkCompressions;

      return featureValue;
    } else {
      throw "Cant't get feature";
    }
  }
}

class Token {
  final String token;

  Token({this.token});

  factory Token.fromJson(Map<String, dynamic> json) {
    return Token(token: json['token']);
  }
}
