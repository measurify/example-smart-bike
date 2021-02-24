/*class Feature {
  List<Docs> docs;
  int totalDocs;
  int limit;
  int totalPages;
  int page;
  int pagingCounter;
  bool hasPrevPage;
  bool hasNextPage;
  Null prevPage;
  int nextPage;

  Feature(
      {this.docs,
      this.totalDocs,
      this.limit,
      this.totalPages,
      this.page,
      this.pagingCounter,
      this.hasPrevPage,
      this.hasNextPage,
      this.prevPage,
      this.nextPage});

  Feature.fromJson(Map<String, dynamic> json) {
    if (json['docs'] != null) {
      docs = new List<Docs>();
      json['docs'].forEach((v) {
        docs.add(new Docs.fromJson(v));
      });
    }
    totalDocs = json['totalDocs'];
    limit = json['limit'];
    totalPages = json['totalPages'];
    page = json['page'];
    pagingCounter = json['pagingCounter'];
    hasPrevPage = json['hasPrevPage'];
    hasNextPage = json['hasNextPage'];
    prevPage = json['prevPage'];
    nextPage = json['nextPage'];
  }

  Map<String, dynamic> toJson() {
    final Map<String, dynamic> data = new Map<String, dynamic>();
    if (this.docs != null) {
      data['docs'] = this.docs.map((v) => v.toJson()).toList();
    }
    data['totalDocs'] = this.totalDocs;
    data['limit'] = this.limit;
    data['totalPages'] = this.totalPages;
    data['page'] = this.page;
    data['pagingCounter'] = this.pagingCounter;
    data['hasPrevPage'] = this.hasPrevPage;
    data['hasNextPage'] = this.hasNextPage;
    data['prevPage'] = this.prevPage;
    data['nextPage'] = this.nextPage;
    return data;
  }
}

class Docs {
  String visibility;
  List<Null> tags;
  String sId;
  String thing;
  String feature;
  String device;
  List<Samples> samples;
  String startDate;
  String endDate;

  Docs(
      {this.visibility,
      this.tags,
      this.sId,
      this.thing,
      this.feature,
      this.device,
      this.samples,
      this.startDate,
      this.endDate});

  Docs.fromJson(Map<String, dynamic> json) {
    visibility = json['visibility'];
    sId = json['_id'];
    thing = json['thing'];
    feature = json['feature'];
    device = json['device'];
    if (json['samples'] != null) {
      samples = new List<Samples>();
      json['samples'].forEach((v) {
        samples.add(new Samples.fromJson(v));
      });
    }
    startDate = json['startDate'];
    endDate = json['endDate'];
  }

  Map<String, dynamic> toJson() {
    final Map<String, dynamic> data = new Map<String, dynamic>();
    data['visibility'] = this.visibility;
    data['_id'] = this.sId;
    data['thing'] = this.thing;
    data['feature'] = this.feature;
    data['device'] = this.device;
    if (this.samples != null) {
      data['samples'] = this.samples.map((v) => v.toJson()).toList();
    }
    data['startDate'] = this.startDate;
    data['endDate'] = this.endDate;
    return data;
  }
}

class Samples {
  List<double> values;

  Samples({this.values});

  Samples.fromJson(Map<String, dynamic> json) {
    values = json['values'].cast<double>();
  }

  Map<String, dynamic> toJson() {
    final Map<String, dynamic> data = new Map<String, dynamic>();
    data['values'] = this.values;
    return data;
  }
}*/
class Feature {
  Feature({
    this.docs,
    this.totalDocs,
    this.limit,
    this.totalPages,
    this.page,
    this.pagingCounter,
    this.hasPrevPage,
    this.hasNextPage,
    this.prevPage,
    this.nextPage,
  });

  List<Docs> docs;
  int totalDocs;
  int limit;
  int totalPages;
  int page;
  int pagingCounter;
  bool hasPrevPage;
  bool hasNextPage;
  dynamic prevPage;
  int nextPage;
}

class Docs {
  Docs({
    this.visibility,
    this.tags,
    this.id,
    this.thing,
    this.feature,
    this.device,
    this.samples,
    this.startDate,
    this.endDate,
  });

  String visibility;
  List<dynamic> tags;
  String id;
  String thing;
  String feature;
  String device;
  List<Sample> samples;
  DateTime startDate;
  DateTime endDate;
}

class Sample {
  Sample({
    this.values,
  });

  List<dynamic> values;
}
