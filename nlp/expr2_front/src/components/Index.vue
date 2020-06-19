<template>
    <el-row type="flex" justify="center">
        <el-col :span="16" style="margin-top: 100px">
            <div class="grid-content">
                <el-card style="margin-bottom: 100px" v-loading="curr_loading">
                    <p>{{curr_poem}}</p>
                    <br>
                        <el-row>
                            <el-button type="primary" value="酒泉子" @click="request('酒泉子')">酒泉子</el-button>
                            <el-button type="primary" value="鹧鸪天" @click="request('鹧鸪天')">鹧鸪天</el-button>
                            <el-button type="primary" value="西江月" @click="request('西江月')">西江月</el-button>
                            <el-button type="primary" value="苏幕遮" @click="request('苏幕遮')">苏幕遮</el-button>
                        </el-row>
                </el-card>
                <el-card v-loading="cut_loading">
                    FMM 结果: <p>{{fmm}}</p>
                    <br>
                    BMM 结果: <p>{{bmm}}</p>
                    <br>
                    <el-input v-model="sentence" placeholder="请输入内容" style="margin-bottom: 10px"></el-input>
                    <el-row>
                        <el-button type="primary" @click="request_cut">切词</el-button>
                    </el-row>
                </el-card>
            </div>
        </el-col>
    </el-row>
</template>

<script>
    import axios from "axios";

    export default {
        name: "Index",
        data() {
            return {
                curr_poem: "",
                curr_loading: false,
                cut_loading: false,
                sentence: "",
                bmm: "",
                fmm: ""
            }
        },
        async mounted() {
            this.curr_poem = "请选择一个词牌名"
        },
        methods: {
            async request(brand_name) {
                /* eslint-disable */
                this.curr_loading = true;
                console.log(brand_name);
                var data = await axios.post("/api/expr1", {"brand_name": brand_name});
                this.curr_poem = data.data.poem;
                this.curr_loading = false;
            },
            async request_cut() {
                /* eslint-disable */
                this.cut_loading = true;
                var data = await axios.post("/api/expr2", {"sentence": this.sentence});
                this.bmm = data.data.bmm;
                this.fmm = data.data.fmm;
                this.cut_loading = false;
            },
        }
    }

</script>

<style>
</style>