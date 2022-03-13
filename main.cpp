#include <iostream>
#include <string>
#include <utility>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

class Point {
public:
  double x;
  double y;
  Point(double _x, double _y): x(_x), y(_y) {}
  friend Point operator + (const Point& lhs, const Point& rhs) {
    return Point(lhs.x + rhs.x, lhs.y + rhs.y);
  }
  friend Point operator * (const double& scala, const Point& point) {
    return Point(point.x * scala, point.y * scala);
  }
  friend Point operator * (const Point& point, const double& scala) {
    return Point(point.x * scala, point.y * scala);
  }
  static Point from_angle(double radius) {
    return Point(cos(radius), sin(radius));
  }
  friend std::ostream& operator << (std::ostream& o, const Point& p) {
    return o << "{x:" << p.x << ", y:" << p.y << "}";
  }
};

template<typename T>
std::ostream& operator << (std::ostream& o, const std::vector<T>& vec) {
    o << "[";
    for (int i = 0; i < vec.size(); ++i) {
      o << vec[i];
      if (i < vec.size() - 1) {
        o << ", ";
      }
    }
    return o << "]";
  }

class RegularPolygon {
public:
  int n; // n >= 3, flat base
  double side_length; // 2 * circum_radius * sin(pi/n)
  double circum_radius; // 1/2 * side_length * csc(pi/n)
  Point center;
  
  RegularPolygon(int _n, int _side_length):
    n(_n),
    side_length(_side_length),
    circum_radius(0.5 * side_length * (1.0 / sin(M_PI / n))),
    center(0, 0) {}
  /**
   *   /\      __
   * /   \   /   \
   * \__/    \__/
   */
  double in_radius() const {
    return circum_radius * cos(M_PI / n);
  }
  double exterior_angle() const {
    return 2 * M_PI / n;
  }
  double interior_angle() const {
    return (n - 2) * M_PI / n;
  }
  std::vector<Point> to_points() const {
    std::vector<Point> res;
    double starter_radius = (M_PI * 1.5) - (M_PI / n);
    for (int i = 0; i < n; ++i) {
      res.push_back(center + Point::from_angle(2.0 * M_PI / n * i + starter_radius) * circum_radius);
    }
    return res;
  }
  Point size() const {
    auto p = to_points();
    auto [xmn, xmx] = std::minmax_element(p.begin(), p.end(), [](auto const& lhs, auto const& rhs) { return lhs.x < rhs.x; });
    auto [ymn, ymx] = std::minmax_element(p.begin(), p.end(), [](auto const& lhs, auto const& rhs) { return lhs.y < rhs.y; });
    return Point(xmx->x - xmn->x, ymx->y - ymn->y);
  }
  friend std::ostream& operator << (std::ostream& o, const RegularPolygon& p) {
    auto size = p.size();
    return o << "{n:" << p.n << ", r:" << p.circum_radius << ", ir:" << p.in_radius() << ", width:" << size.x << ", height:" << size.y << ", center:" << p.center << ", points: " << p.to_points() << "}";
  }
};

int main () {
  std::string inp = "6031301721";
  // std::cin >> inp;
  
  std::vector<RegularPolygon> polys;
  std::sort(inp.begin(), inp.end(), std::greater<char>());
  double y = 0;
  for (char c : inp) {
    RegularPolygon poly(c - '0' + 3, 1.0);
    double height = poly.size().y;
    double base_to_center = poly.in_radius();
    y += base_to_center;
    poly.center.y = y;
    y += height - base_to_center;
    polys.push_back(poly);
  }
  
  std::cout << inp << '\n';
  for (auto& p : polys) {
    std::cout << p << '\n';
  }
  cout << "----\n";
  cout << polys;

  return 0;
}
