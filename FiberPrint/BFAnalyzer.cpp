#include "BFAnalyzer.h"


BFAnalyzer::BFAnalyzer()
{
}


BFAnalyzer::~BFAnalyzer()
{
}


bool BFAnalyzer::SeqPrint()
{
	Init();

	int Nd = ptr_wholegraph_->SizeOfVertList();

	inqueue_.resize(Nd);
	fill(inqueue_.begin(), inqueue_.end(), false);

	return GenerateSeq(1, Nd);
}


bool BFAnalyzer::GenerateSeq(int h, int t)
{
	if (h > t)
	{
		if (TestifySeq())
		{
			GetPrintOrder();
			return true;
		}
		return false;
	}
	for (int i = 0; i < t; i++)
	{
		if (!inqueue_[i])
		{
			print_queue_.push_back({ 0, 0, i });
			inqueue_[i] = true;
			if (GenerateSeq(h + 1, t))
			{
				return true;
			}
			print_queue_.pop_back();
			inqueue_[i] = false;
		}
	}

	return false;
}


bool BFAnalyzer::TestifySeq()
{
	printf("--------------------------------------\n");
	printf("Test on sequence starts.\n");

	int Nd = ptr_wholegraph_->SizeOfVertList();

	D0_.resize(0);
	D0_.setZero();

	angle_state_.clear();
	angle_state_.resize(Nd);
	for (int i = 0; i < Nd; i++)
	{
		angle_state_[i].push_back(0);
		angle_state_[i].push_back(0);
		angle_state_[i].push_back(0);
	}

	for (int i = 0; i < Nd; i++)
	{
		int dual_i = print_queue_[i].dual_id_;
		int orig_i = ptr_wholegraph_->e_orig_id(dual_i);
		WF_edge *e = ptr_frame_->GetEdge(orig_i);

		/* detect floating edge */
		if (!e->isPillar() && !ptr_dualgraph_->isExistingVert(e->pvert_->ID())
			&& !ptr_dualgraph_->isExistingVert(e->ppair_->pvert_->ID()))
		{
			printf("Edge #%d: floating edge detected.\n", i);
			return false;
		}

		/* update structure */
		ptr_dualgraph_->UpdateDualization(e);

		/* testify collision */
		if ((~(angle_state_[dual_i][0] & angle_state_[dual_i][1]
			& angle_state_[dual_i][2])) == 0)
		{
			printf("Edge #%d: test on collision falied.\n", i);
			return false;
		}

		/* testify stiffness */
		if (!TestifyStiffness())
		{
			//if (i == Nd / 2 - 1)
			//{
			//	PrintOutQueue(Nd / 2);
			//	getchar();
			//}
			printf("Edge #%d: test on stiffness falied.\n", i);
			return false;
		}

		/* update collision */
		UpdateStateMap(dual_i, angle_state_);
	}

	return true;
}


void BFAnalyzer::PrintOutQueue(int N)
{	
	string path = ptr_path_;
	string queue_path = path + "/BruteForceQueue.txt";

	FILE *fp = fopen(queue_path.c_str(), "w");

	for (int i = 0; i < N; i++)
	{
		int dual_id = print_queue_[i].dual_id_;
		fprintf(fp, "%d\n", ptr_wholegraph_->e_orig_id(dual_id) / 2);
	}

	fclose(fp);
}