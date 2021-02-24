import 'package:flutter/material.dart';
import 'package:charts_flutter/flutter.dart' as charts;

class AltitudeData {
  final double altitude;
  final int space;

  AltitudeData({this.altitude, this.space});
}

class SpeedData {
  final double speed;

  final int space;

  SpeedData({this.speed, this.space});
}

class AltitudeDataChart extends StatelessWidget {
  final List<AltitudeData> data;
  AltitudeDataChart(this.data);

  @override
  Widget build(BuildContext context) {
    List<charts.Series<AltitudeData, int>> series = [
      charts.Series(
        id: 'Altitude',
        data: data,
        colorFn: (_, __) => charts.MaterialPalette.red.shadeDefault,
        domainFn: (AltitudeData series, _) => series.space,
        measureFn: (AltitudeData series, _) => series.altitude,
      )
    ];
    return Container(
      height: 500,
      padding: EdgeInsets.all(20),
      child: Card(
        child: Padding(
          padding: const EdgeInsets.all(8.0),
          child: Column(
            children: <Widget>[
              Text('Altitude [m]',
                  style: TextStyle(
                      fontSize: 20,
                      fontStyle: FontStyle.italic,
                      fontWeight: FontWeight.w700)),
              Expanded(
                child: charts.LineChart(series),
              ),
            ],
          ),
        ),
      ),
    );
  }
}

class SpeedDataChart extends StatelessWidget {
  final List<SpeedData> data;
  SpeedDataChart(this.data);

  @override
  Widget build(BuildContext context) {
    List<charts.Series<SpeedData, int>> series = [
      charts.Series(
        id: 'Speed',
        data: data,
        colorFn: (_, __) => charts.MaterialPalette.red.shadeDefault,
        domainFn: (SpeedData series, _) => series.space,
        measureFn: (SpeedData series, _) => series.speed,
      )
    ];
    return Container(
      height: 500,
      padding: EdgeInsets.all(20),
      child: Card(
        child: Padding(
          padding: const EdgeInsets.all(8.0),
          child: Column(
            children: <Widget>[
              Text('Speed [km/h]',
                  style: TextStyle(
                      fontSize: 20,
                      fontStyle: FontStyle.italic,
                      fontWeight: FontWeight.w700)),
              Expanded(
                child: charts.LineChart(series),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
