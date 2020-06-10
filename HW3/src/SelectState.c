#include <string.h>
#include <stdlib.h>
#include "Command.h"
#include "SelectState.h"

void field_state_handler(Command_t *cmd, size_t arg_idx) {
    cmd->cmd_args.sel_args.fields = NULL;
    cmd->cmd_args.sel_args.fields_len = 0;

    cmd->cmd_args.sel_args.where=-1;
    int i;
    for(i=0;i<2;i++)
    {
        cmd->cmd_args.sel_args.arg[i]=-1;
        cmd->cmd_args.sel_args.oper[i]=-1;
        cmd->cmd_args.sel_args.data[i]=-1;
    }
    cmd->cmd_args.sel_args.logic=-1;
    cmd->cmd_args.sel_args.field = NULL;
    cmd->cmd_args.sel_args.field_len = 0;
    cmd->cmd_args.sel_args.arrg=-1;

    cmd->cmd_args.sel_args.limit = -1;
    cmd->cmd_args.sel_args.offset = -1;
    while(arg_idx < cmd->args_len) {
        if (!strncmp(cmd->args[arg_idx], "*", 1)) {
            add_select_field(cmd, cmd->args[arg_idx]);
        } else if (!strncmp(cmd->args[arg_idx], "id", 2)) {
            add_select_field(cmd, cmd->args[arg_idx]);
        } else if (!strncmp(cmd->args[arg_idx], "name", 4)) {
            add_select_field(cmd, cmd->args[arg_idx]);
        } else if (!strncmp(cmd->args[arg_idx], "email", 5)) {
            add_select_field(cmd, cmd->args[arg_idx]);
        } else if (!strncmp(cmd->args[arg_idx], "age", 3)) {
            add_select_field(cmd, cmd->args[arg_idx]);
        } else if (!strncmp(cmd->args[arg_idx], "from", 4)) {
            table_state_handler(cmd, arg_idx+1);
            return;
        }
	else if(!strncmp(cmd->args[arg_idx], "avg(id)", 7))
	{
		cmd->cmd_args.sel_args.arrg=1;
		add_aggr_field(cmd, cmd->args[arg_idx]);
	}
	else if(!strncmp(cmd->args[arg_idx], "avg(age)", 8))
	{
		cmd->cmd_args.sel_args.arrg=1;
		add_aggr_field(cmd, cmd->args[arg_idx]);
	}
	else if(!strncmp(cmd->args[arg_idx], "count", 5))
	{
		cmd->cmd_args.sel_args.arrg=1;
		add_aggr_field(cmd, "count");
	}
	else if(!strncmp(cmd->args[arg_idx], "sum(id)", 7))
	{
		cmd->cmd_args.sel_args.arrg=1;
		add_aggr_field(cmd, cmd->args[arg_idx]);
	}
	else if(!strncmp(cmd->args[arg_idx], "sum(age)", 8))
	{
		cmd->cmd_args.sel_args.arrg=1;
		add_aggr_field(cmd, cmd->args[arg_idx]);
	}
	else {
            cmd->type = UNRECOG_CMD;
            return;
        }
        arg_idx += 1;
    }
    cmd->type = UNRECOG_CMD;
    return;
}

void table_state_handler(Command_t *cmd, size_t arg_idx) {
    if (arg_idx < cmd->args_len
            && !strncmp(cmd->args[arg_idx], "table", 5)) {

        arg_idx++;
        if (arg_idx == cmd->args_len) {
            return;
        } else if (!strncmp(cmd->args[arg_idx], "where", 5))
          {
            where_state_handler(cmd, arg_idx+1);
            return;
        } else if (!strncmp(cmd->args[arg_idx], "offset", 6)) {
            offset_state_handler(cmd, arg_idx+1);
            return;
        } else if (!strncmp(cmd->args[arg_idx], "limit", 5)) {
            limit_state_handler(cmd, arg_idx+1);
            return;
        }
    }
    cmd->type = UNRECOG_CMD;
    return;
}


void where_state_handler(Command_t *cmd, size_t arg_idx)
{
    cmd->cmd_args.sel_args.where=1;

    if (arg_idx < cmd->args_len)
    {
	if (!strncmp(cmd->args[arg_idx], "id", 2))
	{
		cmd->cmd_args.sel_args.arg[0]=1;
		arg_idx++;

	        if(!strncmp(cmd->args[arg_idx], "=", 1))
			cmd->cmd_args.sel_args.oper[0]=1;
	        if(!strncmp(cmd->args[arg_idx], "!=", 2))
			cmd->cmd_args.sel_args.oper[0]=2;
	        if(!strncmp(cmd->args[arg_idx], ">", 1))
			cmd->cmd_args.sel_args.oper[0]=3;
	        if(!strncmp(cmd->args[arg_idx], "<", 1))
			cmd->cmd_args.sel_args.oper[0]=4;
	        if(!strncmp(cmd->args[arg_idx], ">=", 2))
			cmd->cmd_args.sel_args.oper[0]=5;
	        if(!strncmp(cmd->args[arg_idx], "<=", 2))
			cmd->cmd_args.sel_args.oper[0]=6;

		arg_idx++;

		cmd->cmd_args.sel_args.data[0]=atof(cmd->args[arg_idx]);
	}
	else if (!strncmp(cmd->args[arg_idx], "name", 4))
	{
		cmd->cmd_args.sel_args.arg[0]=2;
		arg_idx++;

	        if(!strncmp(cmd->args[arg_idx], "=", 1))
			cmd->cmd_args.sel_args.oper[0]=1;
	        if(!strncmp(cmd->args[arg_idx], "!=", 2))
			cmd->cmd_args.sel_args.oper[0]=2;

		arg_idx++;

		strcpy(cmd->cmd_args.sel_args.data1,cmd->args[arg_idx]);
	 }
	else if (!strncmp(cmd->args[arg_idx], "email", 5))
	{
		cmd->cmd_args.sel_args.arg[0]=3;
		arg_idx++;

	        if(!strncmp(cmd->args[arg_idx], "=", 1))
			cmd->cmd_args.sel_args.oper[0]=1;
	        if(!strncmp(cmd->args[arg_idx], "!=", 2))
			cmd->cmd_args.sel_args.oper[0]=2;

		arg_idx++;

		strcpy(cmd->cmd_args.sel_args.data1,cmd->args[arg_idx]);
	 }
	else if (!strncmp(cmd->args[arg_idx], "age", 3))
	{
		cmd->cmd_args.sel_args.arg[0]=4;
		arg_idx++;

	        if(!strncmp(cmd->args[arg_idx], "=", 1))
			cmd->cmd_args.sel_args.oper[0]=1;
	        if(!strncmp(cmd->args[arg_idx], "!=", 2))
			cmd->cmd_args.sel_args.oper[0]=2;
	        if(!strncmp(cmd->args[arg_idx], ">", 1))
			cmd->cmd_args.sel_args.oper[0]=3;
	        if(!strncmp(cmd->args[arg_idx], "<", 1))
			cmd->cmd_args.sel_args.oper[0]=4;
	        if(!strncmp(cmd->args[arg_idx], ">=", 2))
			cmd->cmd_args.sel_args.oper[0]=5;
	        if(!strncmp(cmd->args[arg_idx], "<=", 2))
			cmd->cmd_args.sel_args.oper[0]=6;

		arg_idx++;

		cmd->cmd_args.sel_args.data[0]=atof(cmd->args[arg_idx]);
	}

	arg_idx++;
       
        if (arg_idx == cmd->args_len)
        {
             return;
        }
       else if(!strncmp(cmd->args[arg_idx], "and", 3))
	{
		cmd->cmd_args.sel_args.logic=1;
		
		arg_idx++;
		if (!strncmp(cmd->args[arg_idx], "id", 2))
		{
			cmd->cmd_args.sel_args.arg[1]=1;
			arg_idx++;
	
		        if(!strncmp(cmd->args[arg_idx], "=", 1))
				cmd->cmd_args.sel_args.oper[1]=1;
		        if(!strncmp(cmd->args[arg_idx], "!=", 2))
				cmd->cmd_args.sel_args.oper[1]=2;
		        if(!strncmp(cmd->args[arg_idx], ">", 1))
				cmd->cmd_args.sel_args.oper[1]=3;
		        if(!strncmp(cmd->args[arg_idx], "<", 1))
				cmd->cmd_args.sel_args.oper[1]=4;
		        if(!strncmp(cmd->args[arg_idx], ">=", 2))
				cmd->cmd_args.sel_args.oper[1]=5;
		        if(!strncmp(cmd->args[arg_idx], "<=", 2))
				cmd->cmd_args.sel_args.oper[1]=6;
	
			arg_idx++;
	
			cmd->cmd_args.sel_args.data[1]=atof(cmd->args[arg_idx]);
		}
		else if (!strncmp(cmd->args[arg_idx], "name", 4))
		{
			cmd->cmd_args.sel_args.arg[1]=2;
			arg_idx++;
	
		        if(!strncmp(cmd->args[arg_idx], "=", 1))
				cmd->cmd_args.sel_args.oper[1]=1;
		        if(!strncmp(cmd->args[arg_idx], "!=", 2))
				cmd->cmd_args.sel_args.oper[1]=2;
	
			arg_idx++;
	
			strcpy(cmd->cmd_args.sel_args.data2,cmd->args[arg_idx]);
		 }
		else if (!strncmp(cmd->args[arg_idx], "email", 5))
		{
			cmd->cmd_args.sel_args.arg[1]=3;
			arg_idx++;
	
		        if(!strncmp(cmd->args[arg_idx], "=", 1))
				cmd->cmd_args.sel_args.oper[1]=1;
		        if(!strncmp(cmd->args[arg_idx], "!=", 2))
				cmd->cmd_args.sel_args.oper[1]=2;
	
			arg_idx++;
	
			strcpy(cmd->cmd_args.sel_args.data2,cmd->args[arg_idx]);
		 }
		else if (!strncmp(cmd->args[arg_idx], "age", 3))
		{
			cmd->cmd_args.sel_args.arg[1]=4;
			arg_idx++;
	
		        if(!strncmp(cmd->args[arg_idx], "=", 1))
				cmd->cmd_args.sel_args.oper[1]=1;
		        if(!strncmp(cmd->args[arg_idx], "!=", 2))
				cmd->cmd_args.sel_args.oper[1]=2;
		        if(!strncmp(cmd->args[arg_idx], ">", 1))
				cmd->cmd_args.sel_args.oper[1]=3;
		        if(!strncmp(cmd->args[arg_idx], "<", 1))
				cmd->cmd_args.sel_args.oper[1]=4;
		        if(!strncmp(cmd->args[arg_idx], ">=", 2))
				cmd->cmd_args.sel_args.oper[1]=5;
		        if(!strncmp(cmd->args[arg_idx], "<=", 2))
				cmd->cmd_args.sel_args.oper[1]=6;
	
			arg_idx++;
	
			cmd->cmd_args.sel_args.data[1]=atof(cmd->args[arg_idx]);
		}
	}
	else  if(!strncmp(cmd->args[arg_idx], "or", 2))
	{
		cmd->cmd_args.sel_args.logic=0;

		arg_idx++;
		if (!strncmp(cmd->args[arg_idx], "id", 2))
		{
			cmd->cmd_args.sel_args.arg[1]=1;
			arg_idx++;
	
		        if(!strncmp(cmd->args[arg_idx], "=", 1))
				cmd->cmd_args.sel_args.oper[1]=1;
		        if(!strncmp(cmd->args[arg_idx], "!=", 2))
				cmd->cmd_args.sel_args.oper[1]=2;
		        if(!strncmp(cmd->args[arg_idx], ">", 1))
				cmd->cmd_args.sel_args.oper[1]=3;
		        if(!strncmp(cmd->args[arg_idx], "<", 1))
				cmd->cmd_args.sel_args.oper[1]=4;
		        if(!strncmp(cmd->args[arg_idx], ">=", 2))
				cmd->cmd_args.sel_args.oper[1]=5;
		        if(!strncmp(cmd->args[arg_idx], "<=", 2))
				cmd->cmd_args.sel_args.oper[1]=6;
	
			arg_idx++;
	
			cmd->cmd_args.sel_args.data[1]=atof(cmd->args[arg_idx]);
		}
		else if (!strncmp(cmd->args[arg_idx], "name", 4))
		{
			cmd->cmd_args.sel_args.arg[1]=2;
			arg_idx++;
	
		        if(!strncmp(cmd->args[arg_idx], "=", 1))
				cmd->cmd_args.sel_args.oper[1]=1;
		        if(!strncmp(cmd->args[arg_idx], "!=", 2))
				cmd->cmd_args.sel_args.oper[1]=2;
	
			arg_idx++;
	
			strcpy(cmd->cmd_args.sel_args.data2,cmd->args[arg_idx]);
		 }
		else if (!strncmp(cmd->args[arg_idx], "email", 5))
		{
			cmd->cmd_args.sel_args.arg[1]=3;
			arg_idx++;
	
		        if(!strncmp(cmd->args[arg_idx], "=", 1))
				cmd->cmd_args.sel_args.oper[1]=1;
		        if(!strncmp(cmd->args[arg_idx], "!=", 2))
				cmd->cmd_args.sel_args.oper[1]=2;
	
			arg_idx++;
	
			strcpy(cmd->cmd_args.sel_args.data2,cmd->args[arg_idx]);
		 }
		else if (!strncmp(cmd->args[arg_idx], "age", 3))
		{
			cmd->cmd_args.sel_args.arg[1]=4;
			arg_idx++;
	
		        if(!strncmp(cmd->args[arg_idx], "=", 1))
				cmd->cmd_args.sel_args.oper[1]=1;
		        if(!strncmp(cmd->args[arg_idx], "!=", 2))
				cmd->cmd_args.sel_args.oper[1]=2;
		        if(!strncmp(cmd->args[arg_idx], ">", 1))
				cmd->cmd_args.sel_args.oper[1]=3;
		        if(!strncmp(cmd->args[arg_idx], "<", 1))
				cmd->cmd_args.sel_args.oper[1]=4;
		        if(!strncmp(cmd->args[arg_idx], ">=", 2))
				cmd->cmd_args.sel_args.oper[1]=5;
		        if(!strncmp(cmd->args[arg_idx], "<=", 2))
				cmd->cmd_args.sel_args.oper[1]=6;
	
			arg_idx++;
	
			cmd->cmd_args.sel_args.data[1]=atof(cmd->args[arg_idx]);
		}
	}
	else if (arg_idx < cmd->args_len && !strncmp(cmd->args[arg_idx], "offset", 6))
        {
            offset_state_handler(cmd, arg_idx+1);
            return;
        }
        else if (arg_idx < cmd->args_len && !strncmp(cmd->args[arg_idx], "limit", 5))
        {
             limit_state_handler(cmd, arg_idx+1);
             return;
        }       

	arg_idx++;

        if (arg_idx == cmd->args_len)
        {
             return;
        }
        else if (arg_idx < cmd->args_len && !strncmp(cmd->args[arg_idx], "offset", 6))
        {
            offset_state_handler(cmd, arg_idx+1);
            return;
        }
        else if (arg_idx < cmd->args_len && !strncmp(cmd->args[arg_idx], "limit", 5))
        {
             limit_state_handler(cmd, arg_idx+1);
             return;
        }
    }
    cmd->type = UNRECOG_CMD;
    return;
}


void offset_state_handler(Command_t *cmd, size_t arg_idx) {
    if (arg_idx < cmd->args_len) {
        cmd->cmd_args.sel_args.offset = atoi(cmd->args[arg_idx]);

        arg_idx++;

        if (arg_idx == cmd->args_len) {
            return;
        } else if (arg_idx < cmd->args_len
                && !strncmp(cmd->args[arg_idx], "limit", 5)) {

            limit_state_handler(cmd, arg_idx+1);
            return;
        }
    }
    cmd->type = UNRECOG_CMD;
    return;
}

void limit_state_handler(Command_t *cmd, size_t arg_idx) {
    if (arg_idx < cmd->args_len) {
        cmd->cmd_args.sel_args.limit = atoi(cmd->args[arg_idx]);

        arg_idx++;

        if (arg_idx == cmd->args_len) {
            return;
        }
    }
    cmd->type = UNRECOG_CMD;
    return;
}
