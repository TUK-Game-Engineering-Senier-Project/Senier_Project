#pragma once

#ifndef STDAFX_H_
#define STDAFX_H_

#include <iostream>
#include <random>
#include <thread>
#include <math.h>

#include <mutex> // ���� ������ ���ؽ� ��� 
#include <vector> // ���� ����Ʈ ������
#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���

#include "Objects.h" // ������Ʈ ���� ���� ���

#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����

#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ

#endif 