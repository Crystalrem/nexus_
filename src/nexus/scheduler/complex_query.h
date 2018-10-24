#include "nexus/scheduler/frontend_delegate.h"
#include "nexus/scheduler/scheduler.h"
#include "nexus/scheduler/rps_record.h"
class ComplexQuery {
 public:
  ComplexQuery() {}
  double structure(int n) {
    
    for (int i = 0; i <= n; i++) {
      depth_.push_back(0);
      if(i != 0 && degrees_[i] == 0) {
        redges[i].push_back(0);
        edges[0].push_back(i);
        degrees_[i] ++;
      }
    }
    l = -1, r = 0;
    vector<uint32_t> deg(degrees_);
    node_.resize(n + 1);
    node_[0] = 0;
    uint32_t maxn = 0;
    while(l != r) {
      uint32_t x = node_[++l];
      maxn = std::max(depth_[x], maxn);
      for (int i = 0; i < edges[x].size(); i++) {
        uint32_t y = edges[x][i];
        deg[y] --;
        if(deg[y] == 0) {
          r++;
          depth_[y] = std::max(depth_[y], depth_[x] + 1);
          
          node_.push_back(y);
        }
      }
    }
    return latency_ / maxn;
  }
  ComplexQuery* split() {
    int step = std::max(std::ceil(100000000.0 / latency / latency), 1);
    int m = min(latency / step + 1, 1);
    vector<vector<double> > f;
    vector<vector<double> > g;
    for (int i = 1; i <= n; i++) {
      for
      redgs[]
    }
    
  }
  CtrlStatus init(const LoadDependencyRequest& request) {
    models_.push_back("");
    n = request.n;
    m = request.m;
    latency = request.latency;
    for (int i = 0; i < n; i++) {
      std::model = ModelSessionToString(request.models(i));
      models_id_[model] = i + 1;
      models_.push_back(model);
    }
    edges_.resize(n + 1);
    redges_.resize(n + 1);

    degrees.resize(n + 1);
    for (int i = 0; i <= n; i++) {
      degrees_[i] = 0;
    }
    for (int i = 0; i < m; i++) {
      std::model1 = ModelSessionToString(request.edges(i).v1);
      std::model2 = ModelSessionToString(request.edges(i).v2);
      int v1 = models_id_[model1], v2 = models_id_[model2];
      edges[v1].push_back(v2);
      redges[v2].push_back(v1);
      degrees_[v2] ++;
    }
    double split = structure(n);
    rps.init(models_id_, split);
    return Ctrl_OK;
  }
  void addRecord(const CurRpsRequest& request) {
    rps.add(request);
  }
 private:
  uint32_t n;
  double latency;
  std::map<std::string, uint32_t> models_id_;
  std::vector<vector<uint32_t> > edges_;
  std::vector<vector<uint32_t> > redges_;
  std::vector<std::string> models_;
  std::vector<double> intervals_;
  std::vector<uint32_t> degrees_;
  std::vector<uint32_t> depth_;
  std::vector<uint32_t> node_; 
  double latency_;
  RpsRecord rps;
}
