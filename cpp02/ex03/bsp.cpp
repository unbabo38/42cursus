#include "Point.hpp"

static Fixed sign(Point const p1, Point const p2, Point const p3) {
    return (p2.getX() - p1.getX()) * (p3.getY() - p1.getY()) - (p2.getY() - p1.getY()) * (p3.getX() - p1.getX());
}

bool bsp(Point const a, Point const b, Point const c, Point const point) {
    Fixed d1, d2, d3;

    d1 = sign(a, b, point);
    d2 = sign(b, c, point);
    d3 = sign(c, a, point);

    // 全てが正、あるいは全てが負なら内部にある
    // 0が含まれる場合は辺上か頂点上なので False
    bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos) && (d1 != 0 && d2 != 0 && d3 != 0);
}
