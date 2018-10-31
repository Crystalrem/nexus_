#ifndef NEXUS_SCHEDULER_COMPLEXQUERY_H_
#define NEXUS_SCHEDULER_COMPLEXQUERY_H_

#include "nexus/scheduler/frontend_delegate.h"
#include "nexus/scheduler/scheduler.h"
#include "nexus/scheduler/rps_record.h"
#include "nexus/common/model_db.cpp"
class QuerySplit {

 public:
  QuerySplit() {}
  void addModel(ModelSession model, double lat) {
    models_.push_back(model);
    latencys_.push_back(lat);
  }
  void updateLatencys(std::vector<uint32_t> latencys) {
    for (int i = 0; i < models_.size(); i++) {
      last_latencys_ = latencys_[i];
      latencys_[i] = latencys[i];
    }
  }
  void constructSplit(std::vector<uint32_t> latencys) {
    std::vector<ModelSession> ret;
    for (int i = 0; i < models_.size(); i++) {
      ModelSession tmp = models_[i];
      tmp.set_latency_sla(latencys[i]);
      ret.push_back(tmp);
    }
    return ret;
  }
  
  std::vector<ModelSession> last_subscribe_models() {
    std::vector<ModelSession> ret;
    for (int i = 0; i < models_.size(); i++) {
      ModelSession tmp = models_[i];
      tmp.set_latency_sla(last_latencys[i]);
      ret.push_back(tmp);
    }
    return ret;
  }
  
  std::vector<ModelSession> cur_subscribe_models() {
    std::vector<ModelSession> ret;
    for (int i = 0; i < models_.size(); i++) {
      ModelSession tmp = models_[i];
      tmp.set_latency_sla(latencys[i]);
      ret.push_back(tmp);
    }
    return ret;
  }
  void set_state(bool state) {
    state_ = state;
  }
  bool get_state() {
    return state_;
  }
 private:
  std::vector<ModelSession> models_;
  std::vector<uint32_t> latencys_;
  std::vector<uint32_t> last_latencys_;
  bool state_;
  
}

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
  double gpu_need(std::vector<ModelSession> sess, std::vector<double> rps) {
    double ret = 0;
    for (int i = 0; i < sess.size(); i++) {
      double throughput = max_throughputs_[i + 1][sess[i].latency_sla()].first;
      ret += rps[i] / throughput;
    }
    return ret;
  }
  ComplexQuery* split() {
    std::vector<double>alpha = rps.getRecord();
    int m = latency_ / step_;
    std::vector<std::vector<double> > f, g, last_batch, last_lat;
    std::vector<int> q;
    std::vector<bool> visit;
    std::vector<double> lats;
    std::vector<double> split;
    for (int i = 0; i <= n; i++) {
      visit.push_back(false);
      split.push_back(0.0);
      f.push_back(std::vector<double>());
      g.push_back(std::vector<double>());
      //last_batch.push_back(std::vector<double>());
      last_lat.push_back(std::vector<double>());
      last.push_back(vector)
      if(i == 0) {
        for (int j = 0; j <= m; j++) {
          f[i].push_back(0.0);
        }        
      }
      else {
        for (int j = 0; j <= m; j++) {
          f[i].push_back(max_float);
        }
      }
      for (int j = 0; j <= m; j++) {
        g[i].push_back(0.0);
        //last_batch[i].push_back(0.0);
        last_lat.push_back(0.0);
      }
    }
    for (int i = 1; i <= n; i++) {
      int now = node_[i];
      for(int j = 0; j <= m; j++) {
        for (int k = 0; k < redges_[now].size(); k++) {
          int x = redges_[now][k];
          g[now][j] = max(g[now][j], f[x][j]);
          
        }
        for (int k = 0; k <= m; k++) {
          throughput = max_throughput_[now][j - k].first;
          batch = max_throughput_[now][j - k].second;
          double ngpu = throughput > 0 ? alpha[now] / throughput : max_float;
          double tmp = g[now][k] + ngpu;
          if(f[now][j] > tmp) {
            f[now][j] = tmp;
            //last_batch[now][j] = batch;
            last_lat[now][j] = j - k;
          }
        }
        if(j > 0 && f[now][j] > f[now][j - 1]) {
          f[now][j] = f[now][j - 1];
          //last_batch[now][j] = last_batch[now][j - 1];
          last_lat[now][j] = last_lat[now][j - 1];
        }
      }
    }
    int l = -1, r = -1;
    for (int i = 1; i <= n; i++) {
      if (edge[i].size() == 0) {
        r ++;
        q.push_back(i);
        lats.push_back(m);
        visit[i] = true;
      }
    }
    
    while(l < r) {
      l ++;
      int now = q[l], lat = lats[l];
      if(now == 0) {
        break;
      }
      split[now - 1] = last_lat[now][lat];
      for (int i = 0; i < redge[now].size(); i++) {
        int x = redge[now][i];
        if(visit[x] == false) {
          visit[x] = true;
          r ++;
          q.push_back(x);
          lats.push_back(lat - split[now]);
        }
      }
    }
    //check: if current split is 10% better than last split
    double n1 = gpu_need(qs.cur_subscribe_models(), alpha);
    double n2 = gpu_need(qs.construct_split(split), alpha);
    if(n1 > n2 * 1.1) {
      qs.set_state(true);
      qs.updateLatencys(split);
    }
    else {
      qs.setState(false);
    }
    return qs;
  }
  
  CtrlStatus init(const LoadDependencyRequest& request, std::string common_gpu) {
    common_gpu_ = common_gpu;
    models_.push_back("");
    n = request.n;
    m = request.m;
    latency = request.latency;
    for (int i = 0; i < n; i++) {
      std::model = ModelSessionToString(request.models(i));
      model_sessions_.push_back(request.models(i));
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
    step_ = std::max(std::ceil(100000000.0 / latency / latency), 1);
    int m = latency / step;
    latency_ = step * m;
    max_throughput_.push_back(std::vector<std::pair<float, uint32_t>>());
    for (int i = 0; i < n; i++) {
      std::string profile_id = ModelSessionToProfileID(model_sess);
      auto profile = ModelDatabase::Singleton().GetModelProfile(common_gpu_, profile_id);
      if (profile == nullptr) {
        // Cannot find model profile
        return CTRL_PROFILER_UNAVALIABLE
      }
      max_throughput_.push_back(std::vector<std::pair<float, uint32_t>>());
      max_throughput_[i].push_back(std::make_pair(0.0, 0));
      for (int j = 1; j <= m; j++) {
        int lat = j * step_;
        max_throughput_[i].push_back(profile->GetMaxThroughput(lat));
      }
    }
    double split = structure(n);
    for (int i = 0; i < n; i++) {
      qs.addModel(request.models(i), split);
    }
    rps.init(models_id_, split);
    return CTRL_OK;
  }
  void addRecord(const CurRpsRequest& request) {
    rps.add(request);
  }
 private:
  uint32_t n;
  double latency;
  std::vector<ModelSession> model_sessions_;
  std::map<std::string, uint32_t> models_id_;
  std::vector<std::vector<uint32_t> > edges_;
  std::vector<std::vector<uint32_t> > redges_;
  std::vector<std::string> models_;
  std::vector<double> intervals_;
  std::vector<uint32_t> degrees_;
  std::vector<uint32_t> depth_;
  std::vector<uint32_t> node_; 
  std::string common_gpu_;
  double latency_;
  int step_;  
  std::vector<std::vector<std::pair<float, uint32_t>> > max_throughput_;
  RpsRecord rps;
  QuerySplit qs;
}
