// ---[1번 문제]---

// ShapesApp.cpp 532번째 줄의
GeometryGenerator::MeshData sphere = geoGen.CreateSphere(0.5f, 20, 20); //를
GeometryGenerator::MeshData sphere = geoGen.CreateGeosphere(0.5f, 0); //로 수정한다

2번째 인수로 들어가는 숫자가 증가할수록 (삼각형을 세분하는 횟수가 많아질수록)
더 구에 가까운 도형이 만들어진다.