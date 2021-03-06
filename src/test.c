#include "anim.h" 

#include <glib.h>
#include <math.h>
#include <stdlib.h>

void assert_float_equal(float f1, float f2) {
	g_assert_cmpfloat(fabs(f1-f2), <, FLT_MIN);
}

void test_null() {
    Animation* a = null_animation();
    animation_update(a, 0.0);
    animation_update(a, 1.0);
    assert_float_equal(animation_duration(a), 1.0);
    animation_free(a);
}

void test_linearf_one_dimension_unit() {
    float f=0.0, start = 0.0, end = 1.0;
    Animation* a = linearf(&f, 1, &start, &end);
    assert_float_equal(animation_duration(a), 1.0);

    animation_update(a, 0.0); assert_float_equal(f, 0.0);
    animation_update(a, 0.5); assert_float_equal(f, 0.5);
    animation_update(a, 1.0); assert_float_equal(f, 1.0);
}

void test_linearf_one_dimension_range() {
    float f=0.0, start = 10.0, end = 20.0;
    Animation* a = linearf(&f, 1, &start, &end);
    assert_float_equal(animation_duration(a), 1.0);

    animation_update(a, 0.0); assert_float_equal(f, 10.0);
    animation_update(a, 0.5); assert_float_equal(f, 15.0);
    animation_update(a, 1.0); assert_float_equal(f, 20.0);
}

void test_linearf_three_dimensions_unit() {
    float f[3], start[3] = { 0.0, 0.0, 0.0 }, end[3] = { 1.0, 1.0, 1.0 };
    Animation* a = linearf(f, 3, start, end);
    assert_float_equal(animation_duration(a), 1.0);

    animation_update(a, 0.0); assert_float_equal(f[0], 0.0); assert_float_equal(f[1], 0.0); assert_float_equal(f[2], 0.0);
    animation_update(a, 0.5); assert_float_equal(f[0], 0.5); assert_float_equal(f[1], 0.5); assert_float_equal(f[2], 0.5);
    animation_update(a, 1.0); assert_float_equal(f[0], 1.0); assert_float_equal(f[1], 1.0); assert_float_equal(f[2], 1.0);
}

void test_linearf_three_dimensions_range() {
    float f[3], start[3] = { 10.0, 11.0, 12.0 }, end[3] = { 20.0, 21.0, 22.0 };
    Animation* a = linearf(f, 3, start, end);
    assert_float_equal(animation_duration(a), 1.0);

    animation_update(a, 0.0); assert_float_equal(f[0], 10.0); assert_float_equal(f[1], 11.0); assert_float_equal(f[2], 12.0);
    animation_update(a, 0.5); assert_float_equal(f[0], 15.0); assert_float_equal(f[1], 16.0); assert_float_equal(f[2], 17.0);
    animation_update(a, 1.0); assert_float_equal(f[0], 20.0); assert_float_equal(f[1], 21.0); assert_float_equal(f[2], 22.0);
}

void test_sequence() {
    float f=0.0, start1 = 0.0, end1 = 1.0, start2 = 1.0, end2 = 0.0;
    Animation* a = sequence(linearf(&f, 1, &start1, &end1), linearf(&f, 1, &start2, &end2));
    assert_float_equal(animation_duration(a), 2.0);

    animation_update(a, 0.0); assert_float_equal(f, 0.0);
    animation_update(a, 0.5); assert_float_equal(f, 0.5);
    animation_update(a, 1.0); assert_float_equal(f, 1.0);
    animation_update(a, 1.5); assert_float_equal(f, 0.5);
    animation_update(a, 2.0); assert_float_equal(f, 0.0);
}

void test_parallel() {
    float f1=0.0, f2=0.0, start1 = 0.0, end1 = 1.0, start2 = 1.0, end2 = 0.0;
    Animation* a = parallel(linearf(&f1, 1, &start1, &end1), linearf(&f2, 1, &start2, &end2));
    assert_float_equal(animation_duration(a), 1.0);

    animation_update(a, 0.0); assert_float_equal(f1, 0.0); assert_float_equal(f2, 1.0);
    animation_update(a, 0.5); assert_float_equal(f1, 0.5); assert_float_equal(f2, 0.5);
    animation_update(a, 1.0); assert_float_equal(f1, 1.0); assert_float_equal(f2, 0.0);
}

void test_scale_up() {
    float f=0.0, start = 0.0, end = 1.0;
    Animation* a = scale(linearf(&f, 1, &start, &end), 5.0);
    assert_float_equal(animation_duration(a), 5.0);

    animation_update(a, 0.0); assert_float_equal(f, 0.0);
    animation_update(a, 1.0); assert_float_equal(f, 0.2);
    animation_update(a, 2.0); assert_float_equal(f, 0.4);
    animation_update(a, 3.0); assert_float_equal(f, 0.6);
    animation_update(a, 4.0); assert_float_equal(f, 0.8);
    animation_update(a, 5.0); assert_float_equal(f, 1.0);
}

void test_scale_down() {
    float f=0.0, start = 0.0, end = 1.0;
    Animation* a = scale(linearf(&f, 1, &start, &end), 0.5);
    assert_float_equal(animation_duration(a), 0.5);

    animation_update(a, 0.0); assert_float_equal(f, 0.0);
    animation_update(a, 0.1); assert_float_equal(f, 0.2);
    animation_update(a, 0.2); assert_float_equal(f, 0.4);
    animation_update(a, 0.3); assert_float_equal(f, 0.6);
    animation_update(a, 0.4); assert_float_equal(f, 0.8);
    animation_update(a, 0.5); assert_float_equal(f, 1.0);
}

void test_identity() {
    float f=0.0, start = 0.0, end = 1.0;
    Animation* a = identity(linearf(&f, 1, &start, &end));
    assert_float_equal(animation_duration(a), 1.0);

    animation_update(a, 0.0); assert_float_equal(f, 0.0);
    animation_update(a, 0.5); assert_float_equal(f, 0.5);
    animation_update(a, 1.0); assert_float_equal(f, 1.0);
}

void test_sinusoid() {
    float f=0.0, start = 0.0, end = 1.0;
    Animation* a = sinusoid(linearf(&f, 1, &start, &end));
    assert_float_equal(animation_duration(a), 1.0);

    animation_update(a, 0.0); assert_float_equal(f, 0.0);
    animation_update(a, 0.5); assert_float_equal(f, 0.5);
    animation_update(a, 1.0); assert_float_equal(f, 1.0);
}

void test_reverse() {
    float f=0.0, start = 0.0, end = 1.0;
    Animation* a = reverse(linearf(&f, 1, &start, &end));
    assert_float_equal(animation_duration(a), 1.0);

    animation_update(a, 0.0); assert_float_equal(f, 1.0);
    animation_update(a, 0.5); assert_float_equal(f, 0.5);
    animation_update(a, 1.0); assert_float_equal(f, 0.0);
}

void test_exp() {
    float f=0.0, start = 0.0, end = 1.0;
    Animation* a = exponent(linearf(&f, 1, &start, &end), 2.0);
    assert_float_equal(animation_duration(a), 1.0);

    animation_update(a, 0.0); assert_float_equal(f, 0.0);
    animation_update(a, 0.5); assert_float_equal(f, pow(0.5, 2.0));
    animation_update(a, 1.0); assert_float_equal(f, 1.0);
}

void test_delay() {
    float f=0.0, start = 0.0, end = 1.0;
    Animation* a = delay(linearf(&f, 1, &start, &end), 1.0);
    assert_float_equal(animation_duration(a), 2.0);

    animation_update(a, 0.0); assert_float_equal(f, 0.0);
    animation_update(a, 0.5); assert_float_equal(f, 0.0);
    animation_update(a, 1.0); assert_float_equal(f, 0.0);
    animation_update(a, 1.5); assert_float_equal(f, 0.5);
    animation_update(a, 2.0); assert_float_equal(f, 1.0);
}

void scenario_one() {
	float x=0.0, y=0.0;
	Animation* a = parallel(sequence(scale(linearf1(&x, 0, 3), 3), reverse(linearf1(&x, 1, 3))),
                            scale(linearf1(&y, 1, 5), 4));

    animation_update(a, 0.0); assert_float_equal(x, 0.0); assert_float_equal(y, 1.0);
    animation_update(a, 0.5); assert_float_equal(x, 0.5); assert_float_equal(y, 1.5);
    animation_update(a, 1.0); assert_float_equal(x, 1.0); assert_float_equal(y, 2.0);
    animation_update(a, 1.5); assert_float_equal(x, 1.5); assert_float_equal(y, 2.5);
    animation_update(a, 2.0); assert_float_equal(x, 2.0); assert_float_equal(y, 3.0);
    animation_update(a, 2.5); assert_float_equal(x, 2.5); assert_float_equal(y, 3.5);
    animation_update(a, 3.0); assert_float_equal(x, 3.0); assert_float_equal(y, 4.0);
    animation_update(a, 3.5); assert_float_equal(x, 2.0); assert_float_equal(y, 4.5);
    animation_update(a, 4.0); assert_float_equal(x, 1.0); assert_float_equal(y, 5.0);

    animation_free(a);
}

void test_bezier_one_dimension_one() {
    int i;
    float v = 0.0;

    float** c = malloc(sizeof(float*) * 2);
    for (i=0; i<2; i++)
        c[i] = malloc(sizeof(float));

    c[0][0] = 0.0;
    c[1][0] = 1.0;

    Animation* a = bezierf(&v, 1, 2, c);

    animation_update(a, 0.0); assert_float_equal(v, 0.0);
    animation_update(a, 0.5); assert_float_equal(v, 0.5);
    animation_update(a, 1.0); assert_float_equal(v, 1.0);

    animation_free(a);
}

void test_bezier_one_dimension_two() {
    int i;
    float v = 0.0;

    float** c = malloc(sizeof(float*) * 3);
    for (i=0; i<3; i++)
        c[i] = malloc(sizeof(float));

    c[0][0] = 0.0;
    c[1][0] = 1.0;
    c[2][0] = 0.0;

    Animation* a = bezierf(&v, 1, 3, c);

    animation_update(a, 0.0); assert_float_equal(v, 0.0);
    animation_update(a, 0.5); assert_float_equal(v, 0.5);
    animation_update(a, 1.0); assert_float_equal(v, 0.0);

    animation_free(a);
}

void test_bezier_three_dimension_one() {
    int i;
    float v[3] = { 0.0, 0.0, 0.0 };

    float** c = malloc(sizeof(float*) * 3);
    for (i=0; i<3; i++)
        c[i] = malloc(sizeof(float) * 3);

    c[0][0] = 0.0; c[0][1] = 0.0; c[0][2] = 0.0;
    c[1][0] = 1.0; c[1][1] = 2.0; c[1][2] = 3.0;
    c[2][0] = 0.0; c[2][1] = 0.0; c[2][2] = 0.0;

    Animation* a = bezierf(v, 3, 3, c);

    animation_update(a, 0.0); assert_float_equal(v[0], 0.0); assert_float_equal(v[1], 0.0); assert_float_equal(v[2], 0.0);
    animation_update(a, 0.5); assert_float_equal(v[0], 0.5); assert_float_equal(v[1], 1.0); assert_float_equal(v[2], 1.5);
    animation_update(a, 1.0); assert_float_equal(v[0], 0.0); assert_float_equal(v[1], 0.0); assert_float_equal(v[2], 0.0);

    animation_free(a);
}

int main(int argc, char** argv) {
    g_test_init(&argc, &argv, NULL);
    g_test_add_func("/libanim/animation/null", test_null);
    g_test_add_func("/libanim/animation/linearf/one_dimension/1", test_linearf_one_dimension_unit);
    g_test_add_func("/libanim/animation/linearf/one_dimension/2", test_linearf_one_dimension_range);
    g_test_add_func("/libanim/animation/linearf/three_dimensions/1", test_linearf_three_dimensions_unit);
    g_test_add_func("/libanim/animation/linearf/three_dimensions/2", test_linearf_three_dimensions_range);
    g_test_add_func("/libanim/animation/bezier/one_dimension/1", test_bezier_one_dimension_one);
    g_test_add_func("/libanim/animation/bezier/one_dimension/2", test_bezier_one_dimension_two);
    g_test_add_func("/libanim/animation/bezier/three_dimensions/1", test_bezier_three_dimension_one);
    g_test_add_func("/libanim/animation/sequence", test_sequence);
    g_test_add_func("/libanim/animation/parallel", test_parallel);
    g_test_add_func("/libanim/animation/delay", test_delay);
    g_test_add_func("/libanim/animation/scale/1", test_scale_up);
    g_test_add_func("/libanim/animation/scale/2", test_scale_down);
    g_test_add_func("/libanim/transform/identity", test_identity);
    g_test_add_func("/libanim/transform/sinusoid", test_sinusoid);
    g_test_add_func("/libanim/transform/reverse", test_reverse);
    g_test_add_func("/libanim/transform/exp", test_exp);
    g_test_add_func("/libanim/scenario", scenario_one);
	g_test_run();

	return 0;
}
