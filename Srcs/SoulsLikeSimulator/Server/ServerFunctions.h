#pragma once

// <���� ��� ��� ����>

// Ŭ���̾�Ʈ�� ������ ��� �Լ�
DWORD WINAPI ProcessClient(LPVOID arg);

// Ű ���� ���� �Լ�
void SendKeyInput(g_sockInfo* sock_info, char input, bool KeyDown);

// �÷��̾� ���� ������Ʈ �Լ�
void UpdatePlayer();