#include"Enemy.h"
#include"ModelManager.h"
#include"ImageManager.h"
#include<fstream>



void Enemy::LoadData() {
	std::ifstream file;
	file.open("Resources/csv/EnemyStatus.csv");
	assert(file.is_open());

	parameterCommands << file.rdbuf();
	
	file.close();
}

void Enemy::UpdateCommand() {
	std::string line;

	while (getline(parameterCommands,line)) {
		//��͂��₷������
		std::istringstream line_stream(line);

		std::string word;
		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		getline(line_stream,word,',');
		//"//"����n�܂�s�̓R�����g
		if (word.find("//")==0) {
			//��΂�
			continue;
		}
		//�e�R�}���h
		if (word.find("HP")==0) {
			getline(line_stream, word, ',');
			hp = (float)std::atof(word.c_str());
		}else if(word.find("VEL")==0) {
			getline(line_stream, word, ',');
			vel = (float)std::atof(word.c_str());
		} else if (word.find("PHASE") == 0) {
			getline(line_stream, word, ',');
			if (word.find("APPROCH") == 0) {
				phase_ = Enemy::Phase::Approch;
			}else if (word.find("LEAVE") == 0) {
				phase_ = Enemy::Phase::Leave;
			}else if (word.find("WAIT") == 0) {
				phase_ = Enemy::Phase::Wait;
			} else {
				assert(0);//�t�F�[�Y����������
			}
		} else if (word.find("WAITTIMER") == 0) {
			getline(line_stream, word, ',');
			waitTimer = (float)std::atof(word.c_str());
			break;
		}
	}
}

void Enemy::DebugUpdate() {
}

void Enemy::OnInit() {
	obj->SetRotation(XMFLOAT3{0,0,0});
	LoadData();
	UpdateCommand();
}

void Enemy::OnUpda() {
	PhaseMove();
}

void Enemy::OnDraw() {
}

void Enemy::OnFinal() {
}

void Enemy::PhaseMove() {
	switch (phase_) {
	case Enemy::Phase::Approch :
		ApprochUpdate();
		break;
	case Enemy::Phase::Leave:
		LeaveUpdate();
		break;
	case Enemy::Phase::Wait:
		WaitUpdate();
		break;
	default:
		break;
	}
}

void Enemy::ApprochUpdate() {

}

void Enemy::LeaveUpdate() {

}

void Enemy::WaitUpdate() {

}

